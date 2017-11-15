#include "Globals.h"
#include "Application.h"   
#include ".\mmgr\mmgr.h"
#include "ModuleImporter.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "CompTransform.h"
#include "ComponentCamera.h"
#include "ModuleScene.h"
#include "parson.h"
#include <ctime>

#include "ModuleResourceManager.h"
#include "ResourceMesh.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"


#pragma comment (lib, "3DEngine/Assimp/libx86/assimp.lib")
#pragma comment(lib, "3DEngine/Devil/libx86/DevIL.lib")
#pragma comment(lib, "3DEngine/Devil/libx86/ILU.lib")
#pragma comment(lib, "3DEngine/Devil/libx86/ILUT.lib")

ModuleImporter::ModuleImporter()
{

}

ModuleImporter::ModuleImporter(bool start_enabled) : Module(start_enabled)
{
	name = "importer";
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Init(const JSON_Object* config_data)
{
	bool ret = true;

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);


	//Init DevIL libs
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

	ILuint devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		LOG("Devil Error (ilInit: %s)", iluErrorString(devilError));
	}

	//Check DevIL version
	else if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION || iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION || ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION)
	{
		LOG("DevIL version is different!");
	}
	else LOG("DevIL succesfully loaded");
	
	return ret;
}

bool ModuleImporter::CleanUp()
{

	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

//IMPORT METHODS--------------------------------------------------------------

//Import scenes to asset folder
bool ModuleImporter::ImportFBX(const char * path)
{
	bool ret = true;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	FBX_data fbx_d;

	if (scene != nullptr)
	{
		//For un-named meshes
		mesh_id = 0;

		ImportScene(path, scene, fbx_d);
		aiReleaseImport(scene);
		
		CreateFBXmeta(fbx_d, path);
	}
	else
	{
		LOG("Error importing fbx %s: scene == nullptr", path);
		ret = false;
	}

	return ret;
}

//Search all materials and meshes in a fbx scene
void ModuleImporter::ImportScene(const char* path, const aiScene * scene, FBX_data &fbx_d)
{
	//Store all materials in a vector and assign a id of -1
	if (scene->HasMaterials())
	{
		for (int i = 0; i < scene->mNumMaterials; i++)
		{
			ImportTextureFromFBX(scene, path, i, fbx_d );
		}
	}

	if (scene->HasMeshes())
	{
		std::string scene_name = std::experimental::filesystem::path(path).stem().string().c_str();

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			ImportMeshFromFBX(scene->mMeshes[i], scene_name.c_str(), fbx_d);
		}
	}
}

//Imports texture to asset 
int ModuleImporter::ImportTextureFromFBX(const aiScene* scene, const char* path, int material_index, FBX_data& fbx_d)
{
	uint text_id = 0;
	aiString t;
	scene->mMaterials[material_index]->GetTexture(aiTextureType_DIFFUSE, 0, &t);
	std::string	texture_name = t.C_Str();
	std::string texture_path = path;

	//Construc the general path for the texture
	while (texture_path.back() != '\\')
	{
		texture_path.pop_back();
	}
	texture_path += texture_name;

	fbx_d.texture_names.push_back(texture_name.c_str());
		
	std::string texture_asset_file = App->fs->CreateDirectoryInAssets("Textures");
	texture_asset_file += texture_name;
	App->fs->CloneFile(texture_path.c_str(), texture_asset_file.c_str());
	
	return text_id;
}

//Imports meshes to assets
std::string ModuleImporter::ImportMeshFromFBX(const aiMesh* m, const char* scene_name, FBX_data& fbx_d)
{
	bool ret = true;


	//Data to fill------------------------------
	std::vector<float> vertices;
	std::vector<uint> indices;
	std::vector<float> normals;
	std::vector<float> texture_coords;

	int numVertx = m->mNumVertices;
	int numInd = m->mNumFaces * 3;
	//------------------------------------------

	//Copy vertices
	for (int i = 0; i < numVertx; i++)
	{
		vertices.push_back(m->mVertices[i].x);
		vertices.push_back(m->mVertices[i].y);
		vertices.push_back(m->mVertices[i].z);
	}

	if (vertices.empty())
	{
		LOG("ERROR: Loading mesh without vertex data");
		ret = false;
	}

	//Copy indices
	if (m->HasFaces())
	{
		for (int k = 0; k < m->mNumFaces; k++)
		{
			if (m->mFaces[k].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				for (int j = 0; j < m->mFaces[k].mNumIndices; j++)
				{
					indices.push_back(m->mFaces[k].mIndices[j]);
				}
			}
		}
	}


	if (indices.empty())
	{
		LOG("ERROR: Loading mesh without vertex data");
		ret = false;
	}

	//Copy normals
	if (m->HasNormals())
	{
		for (int i = 0; i < numVertx; i++)
		{
			normals.push_back(m->mNormals[i].x);
			normals.push_back(m->mNormals[i].y);
			normals.push_back(m->mNormals[i].z);
		}
	}
	else LOG("WARNING: Importing mesh without normals");


	//copy texture coords
	if (m->HasTextureCoords(0))
	{
		for (int i = 0; i < numVertx; i++)
		{
			texture_coords.push_back(m->mTextureCoords[0][i].x);
			texture_coords.push_back(m->mTextureCoords[0][i].y);
		}
	}
	else LOG("WARNING: Importing mesh without texture coords");

	//save mesh to .carca

	std::string name = m->mName.C_Str();

	//Put generic name if name is empty
	if (name.empty())
	{
		name = scene_name;
		name += "_mesh_";
		name += std::to_string(mesh_id);
	}
	mesh_id++;


	if (ret)
	{
		name += FORMAT_EXTENSION;
		fbx_d.mesh_names.push_back(name);
		std::string asset_folder = App->fs->GetAssetDirectory();
		asset_folder += App->fs->CreateDirectoryInAssets("Meshes") + name;
		
		//Create tmp mesh to save to binary
		ResourceMesh mesh(0);
		mesh.SetData(vertices.data(),indices.data(), numVertx, numInd, normals.data(), texture_coords.data());

		SaveMeshToOwnFormat(asset_folder.c_str(), &mesh);

		//Set data to nullptr to avoid exceptions
		mesh.SetDataToNullptr();
		
	}

	return name;
}

//Create meta files
void ModuleImporter::CreateFBXmeta(FBX_data &fbx_d, const char* path) const
{
	//Extract file name
	std::string meta_path = path;
	size_t begin_name = meta_path.find_last_of('\\');
	std::string file_name = meta_path.substr(begin_name +1);
	meta_path += META_EXTENSION;

	//Serialize to JSON
	JSON_Value * meta_file = json_value_init_object();	
	JSON_Object* obj_data = json_value_get_object(meta_file);

	
	json_object_dotset_string(obj_data, "FBX name", file_name.c_str());	

	std::time_t result = std::time(nullptr);
	std::string timestamp = std::asctime(std::localtime(&result));
	
	json_object_dotset_string(obj_data, "Time Stamp", timestamp.c_str());

	//Serialize mesh names
	json_object_set_value(obj_data, "Mesh names", json_value_init_object());	
	JSON_Object* mesh_name = json_object_get_object(obj_data, "Mesh names");
	for (int i = 0; i < fbx_d.mesh_names.size(); i++)
	{
		std::string mesh_number = "Mesh ";
		mesh_number += std::to_string(i);
		json_object_dotset_string(mesh_name, mesh_number.c_str(), fbx_d.mesh_names[i].c_str());
	}

	//Serialize texture names
	json_object_set_value(obj_data, "Texture names", json_value_init_object());
	JSON_Object* texture_name = json_object_get_object(obj_data, "Texture names");
	for (int i = 0; i < fbx_d.texture_names.size(); i++)
	{
		std::string text_number = "Texture ";
		text_number += std::to_string(i);
		json_object_dotset_string(texture_name, text_number.c_str(), fbx_d.texture_names[i].c_str());
	}

	

	json_serialize_to_file(meta_file, meta_path.c_str());

}





//LOAD AND SAVE METHODS --------------------------------------------------------------------------------

//Load fbx hierarchy in scene
bool ModuleImporter::LoadFBX(const char * path)
{
	bool ret = true;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	//For un-named meshes
	mesh_id = 0;
	if (scene != nullptr)
	{
		std::string scene_name = std::experimental::filesystem::path(path).stem().string().c_str();
		
		//Creates scene hierarchy
		for(int i=0;i<scene->mRootNode->mNumChildren;i++)
			SearchFBXNode(scene->mRootNode->mChildren[i], scene, nullptr, scene_name.c_str());

		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", path);
		ret = false;
	}

	return ret;
}

//Searches every FBX node for data and creates one GameObject per node
std::string ModuleImporter::SearchFBXNode(const aiNode* n, const aiScene* scene, GameObject* parent, const char* scene_name)
{
	aiVector3D ai_location;
	aiVector3D ai_scale;
	aiQuaternion ai_rotation;

	//Decompose transformation matrix
	n->mTransformation.Decompose(ai_scale, ai_rotation, ai_location);

	float3 location;
	float3 scale;
	math::Quat rot;

	location.x = ai_location.x;
	location.y = ai_location.y;
	location.z = ai_location.z;
	scale.x = ai_scale.x;
	scale.y = ai_scale.y;
	scale.z = ai_scale.z;
	rot.x = ai_rotation.x;
	rot.y = ai_rotation.y;
	rot.z = ai_rotation.z;
	rot.w = ai_rotation.w;

	//Create the object
	GameObject* new_obj = App->scene->CreateGameObject(n->mName.C_Str(), parent);
	new_obj->CreateComponent_Transform(location, scale, rot);


	//Load all meshes and store the last texture id
	int last_material_index = 0;
	for (int i = 0; i < n->mNumMeshes; i++)
	{
		std::string mesh_name = scene->mMeshes[n->mMeshes[i]]->mName.C_Str();
		//Put generic name if name is empty
		if (mesh_name.empty())
		{
			mesh_name = scene_name;
			mesh_name += "_mesh_";
			mesh_name += std::to_string(n->mMeshes[i]);
		}
		//Call resource manager to load mesh
		std::string mesh_file_path = App->fs->GetAssetDirectory();
		mesh_file_path += "Meshes/" + mesh_name + FORMAT_EXTENSION;
		ResourceMesh* mesh = (ResourceMesh*)App->resource_m->LoadResource(mesh_file_path.c_str());
		new_obj->CreateComponent_Mesh(mesh_name.c_str(),mesh);

		last_material_index = scene->mMeshes[n->mMeshes[i]]->mMaterialIndex;
	}


	//Call resource manager to load texture
	std::string texture_path = ExtractTextureName(scene->mMaterials[last_material_index]);
	ResourceTexture* t = (ResourceTexture*)App->resource_m->LoadResource(texture_path.c_str());	
	new_obj->CreateComponent_Material(t);


	//Searches for children nodes
	for (int i = 0; i < n->mNumChildren; i++)
		SearchFBXNode(n->mChildren[i], scene, new_obj, scene_name);
	
	return n->mName.C_Str();
}

//Small utility for extraction texture path in assets
std::string ModuleImporter::ExtractTextureName(const aiMaterial* material)
{
	
	aiString s;
	material->GetTexture(aiTextureType_DIFFUSE, 0, &s);
	std::string texture_name = std::experimental::filesystem::path(s.C_Str()).stem().string().c_str();
	std::string texture_extension = std::experimental::filesystem::path(s.C_Str()).extension().string().c_str();
	std::string texture_path = App->fs->GetAssetDirectory();
	texture_path += "Textures/";	
	texture_path += texture_name + texture_extension;

	return texture_path;
}


//Load texture from image
uint ModuleImporter::LoadTexture(const char * path, bool flip) const
{
	//Gen image
	ILuint img_id = 0;
	ilGenImages(1, &img_id);
	ilBindImage(img_id);

	//load from path
	ilLoadImage(path);

	ILuint devilError1 = ilGetError();
	if (devilError1 != IL_NO_ERROR)
	{
		LOG("Devil Error (ilInit: %s)", iluErrorString(devilError1));
		return 0;
	}


	if(flip)
		iluFlipImage();
	
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);


	ILuint devilError2 = ilGetError();
	if (devilError2 != IL_NO_ERROR)
	{
		LOG("Devil Error (ilInit: %s)", iluErrorString(devilError2));
		return 0;
	}

	//Send texture to GPU
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &img_id);
	glBindTexture(GL_TEXTURE_2D, img_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
		0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

	ILuint devilError3 = ilGetError();
	if (devilError3 != IL_NO_ERROR)
	{
		LOG("Devil Error (ilInit: %s)", iluErrorString(devilError3));
		return 0;
	}


	LOG("Loaded Texture Successfully");
	return img_id;
}

//Store texture in library
void ModuleImporter::SaveTextureToDDS(const char * path) const
{
	ILuint size;
	ILubyte *data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
	if (size > 0) {
		data = new ILubyte[size]; // allocate data buffer
		if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
		{
			App->fs->SaveDataTo((char*)data, size, path);
		}

		delete[] data;
	}
}

//Save mesh to .carca in given path
bool ModuleImporter::SaveMeshToOwnFormat(const char* path, ResourceMesh* mesh)const
{
	bool ret = true;

	//DATA ORDER: tag - num vertex - num index - vertex - index - has normals - has text coords - normals - text coords

	uint num_vert = mesh->GetNumVertices();
	const float* vert = mesh->GetVertices();
	uint num_ind = mesh->GetNumIndices();
	const uint* ind = mesh->GetIndices();
	const float* normals = mesh->GetNormals();
	const float* texture_coords = mesh->GetTextureCoords();


	bool has_normals = false;
	bool has_text_coords = false;


	//calculate base size
	uint size = sizeof(uint) * 3 + sizeof(float) * num_vert * 3 + sizeof(uint) * num_ind + sizeof(bool) * 2 + 1;

	if (normals)
	{
		has_normals = true;
		size += sizeof(float)*num_vert * 3;
	}
	if (texture_coords)
	{
		has_text_coords = true;
		size += sizeof(float)*num_vert * 2;
	}


	char* data = new char[size];
	char* cursor = data;

	//Set end of data
	data[size - 1] = '\0';

	uint ranges[3] = { MESH_SAVETAG, num_vert, num_ind };
	uint size_of = sizeof(ranges);


	//Copy num vert & num ind
	memcpy(cursor, ranges, size_of);
	cursor += size_of;


	//Copy vert
	size_of = sizeof(float)*num_vert * 3;
	memcpy(cursor, vert, size_of);
	cursor += size_of;

	//Copy ind
	size_of = sizeof(uint)*num_ind;
	memcpy(cursor, ind, size_of);
	cursor += size_of;

	//Copy normals and text coords
	bool has[2] = { has_normals, has_text_coords };

	size_of = sizeof(bool);
	memcpy(cursor, &has[0], size_of);
	cursor += size_of;

	if (has_normals)
	{
		size_of = sizeof(float)*num_vert * 3;
		memcpy(cursor, normals, size_of);
		cursor += size_of;
	}

	size_of = sizeof(bool);
	memcpy(cursor, &has[1], size_of);
	cursor += size_of;

	if (has_text_coords)
	{
		size_of = sizeof(float)*num_vert * 2;
		memcpy(cursor, texture_coords, size_of);
		cursor += size_of;
	}
	//save meshes in path
	App->fs->SaveDataTo(data, size, path);	
	
	delete[] data;

	return ret;
}

//Creates the resource with the given ID
ResourceMesh* ModuleImporter::LoadMeshFromOwnFormat(const char * path, uint UID) const
{
	char* data = nullptr;

	if (App->fs->LoadDataFrom(data, path) == false)
	{
		return nullptr;
	}	

	//Get data from buffer---------------
	//DATA ORDER: num vertex - num index - vertex - index - has normals - has text coords - normals - text coords

	//Data to load
	uint num_vert = 0;
	uint num_ind = 0;

	bool has_normals = false;
	bool has_text_coords = false;
	
	//Get tag and check that its a mesh
	char* cursor = data;
	uint tag[] = { -1 };
	uint size_of = sizeof(uint);
	memcpy(tag, cursor, size_of);

	if (*tag != MESH_SAVETAG)
	{
		LOG("ERROR: this is not a mesh");
		return nullptr;
	}
	cursor += size_of;

	//If OK, load num vertx and indx
	uint ranges[2];
	size_of = sizeof(ranges);
	memcpy(ranges, cursor, size_of);
	num_vert = ranges[0];
	num_ind = ranges[1];
	cursor += size_of;

	//Load vertx
	float* vert = new float[num_vert * 3];
	size_of = sizeof(float)*num_vert * 3;
	memcpy(vert, cursor, size_of);
	cursor += size_of;

	//Load ind
	unsigned int* ind = new unsigned int[num_ind];
	size_of = sizeof(float)*num_ind;
	memcpy(ind, cursor, size_of);
	cursor += size_of;

	//Load normals and text coords
	bool hases[2];

	float* normals = nullptr;
	float* texture_coord = nullptr;

	//has normals
	size_of = sizeof(bool);
	memcpy(&hases[0], cursor, size_of);
	cursor += size_of;
	if (hases[0])
	{
		normals = new float[num_vert * 3];
		size_of = sizeof(float) * num_vert * 3;
		memcpy(normals, cursor, size_of);
		cursor += size_of;
	}

	//has text coords
	size_of = sizeof(bool);
	memcpy(&hases[1], cursor, size_of);
	cursor += size_of;
	if (hases[1])
	{
		texture_coord = new float[num_vert * 2];
		size_of = sizeof(float) * num_vert * 2;
		memcpy(texture_coord, cursor, size_of);
		cursor += size_of;
	}


	LOG("Loaded %s mesh successfully", name);
	delete[] data;

	//Create New resource
	ResourceMesh* resource_mesh = (ResourceMesh*)App->resource_m->CreateResource(Resource::MESH, UID);
	resource_mesh->SetData(vert, ind, num_vert, num_ind, normals, texture_coord);

	return resource_mesh;
}





