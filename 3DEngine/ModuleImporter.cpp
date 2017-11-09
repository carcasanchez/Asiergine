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


//Load General file and identify the format
void ModuleImporter::LoadFile(const char * path)
{
	std::string tmp = path;
	std::string extension;

	LOG("-----------------------Loading file from %s", path);
	
	//Get file extension (reversed)
	while (tmp.back() != '.')
	{
		extension.push_back(tmp.back());
		tmp.pop_back();

		if (tmp.empty())
			break;
	}


	//Normalize to lower case
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
	//Reverse to be human readable
	std::reverse(extension.begin(), extension.end());


	//Check if extension is valid
	if (extension.compare("fbx") == 0)
	{
		LoadFBX(path);
	}
	else if (extension.compare("png") || extension.compare("jpg") || extension.compare("dds") || extension.compare("tga"))
	{
		GLuint text_id = LoadTexture(path);
		SaveTextureToDDS(std::experimental::filesystem::path(path).stem().string().c_str());
		glDeleteTextures(1, &text_id);
	}
	else LOG("ERROR: File extension '.%s' not allowed", extension.c_str());

}


//Load FBX scene----------------------------------
bool ModuleImporter::LoadFBX(const char * path)
{
	bool ret = true;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	
	if (scene != nullptr)
	{
		fbx_path = path;
		ImportScene(scene);
		aiReleaseImport(scene);	

		materials.clear();
	}
	else
	{
		LOG("Error loading scene %s", path);
		ret = false;
	}

	return ret;
}


//Iterates all nodes saving materials and meshes
void ModuleImporter::ImportScene(const aiScene * scene)
{	
	//Store all materials in a vector and assign a id of -1
	if (scene->HasMaterials())
	{
		for (int i = 0; i < scene->mNumMaterials; i++)
		{
			std::pair<aiMaterial*, int> pair;
			pair.first = scene->mMaterials[i];
			pair.second = -1;
			materials.push_back(pair);
		}
	}
	
	if(scene->HasMeshes())
		for (int i = 0; i < scene->mRootNode->mNumChildren; i++)
		{
			SearchNode(scene->mRootNode->mChildren[i], scene, App->scene->root);
		}
}

//Searches every FBX node for data and creates one GameObject per node
std::string ModuleImporter::SearchNode(const aiNode* n, const aiScene* scene, GameObject* parent)
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

	//For meshes without name
	mesh_id = 0;


	//Create the object
	GameObject* new_obj = App->scene->CreateGameObject(n->mName.C_Str(), parent);
	new_obj->CreateComponent_Transform(location, scale, rot);


	//Saves all meshes to .carca
	int last_material_index = 0;
	for (int i = 0; i < n->mNumMeshes; i++)
	{
		aiMesh* m = scene->mMeshes[n->mMeshes[i]];
		std::string mesh_name = ImportGeometry(m, n->mName.C_Str());
		LoadMeshFromOwnFormat(mesh_name.c_str(), new_obj);
		last_material_index = m->mMaterialIndex;
	}

	//Searches and loads texture
	std::string texture_name;
	int text_id = SearchForTexture(scene, fbx_path.c_str(), last_material_index, texture_name);

	ResourceTexture* t = (ResourceTexture*)App->resource_m->CreateResource(Resource::TEXTURE);
	t->SetData(text_id, std::experimental::filesystem::path(texture_name).stem().string().c_str());
	new_obj->CreateComponent_Material(t);


	//Searches for children nodes
	for (int i = 0; i < n->mNumChildren; i++)
		SearchNode(n->mChildren[i], scene, new_obj);
	
	return n->mName.C_Str();
}


//Loads meshes from FBX node
std::string ModuleImporter::ImportGeometry(const aiMesh* m, const char* obj_name)
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
	if(m->HasFaces())
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
		for (int i = 0; i < numVertx ; i++)
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
		name = obj_name;
		name += "_mesh_";
		name += std::to_string(mesh_id);
		mesh_id++;
	}

	if(ret)
		SaveMeshToOwnFormat(name.c_str(), numVertx, numInd, vertices.data(), indices.data(), normals.data(), texture_coords.data());
	

	return name;
}



//Loads all textures of the fbx in memory. Stores the ID 
int ModuleImporter::SearchForTexture(const aiScene* scene, const char* path, int material_index, std::string &texture_name)
{
	int text_id = 0;	
	
	if (!materials.empty())		
	{	
		aiString s;
		materials[material_index].first->GetTexture(aiTextureType_DIFFUSE, 0, &s);
		texture_name = s.C_Str();
		std::string  geom_path = path;

		//Construc the general path for the texture
		while (geom_path.back() != '\\')
		{
			geom_path.pop_back();
		}
		geom_path += texture_name;

		//Extract the extension to texture name
		texture_name = std::experimental::filesystem::path(texture_name).stem().string().c_str();

		//If texture already loaded, don't load again
		if (materials[material_index].second == -1)
		{
			

			//Load texture and get ID
			text_id = LoadTexture(geom_path.c_str());
			//Store texture Library in DDS format
			SaveTextureToDDS(texture_name.c_str());

			materials[material_index].second = text_id;

			if (text_id == 0)
			{
				LOG("Warning: --------Scene missing textures");
			}
		}
		else
		{
			text_id = materials[material_index].second;
		};
	}
	return text_id;
}


//Load texture from image-----------------------------
uint ModuleImporter::LoadTexture(const char * path, bool from_scene) const
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

	
	// If the image is flipped
	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();		
	}


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

void ModuleImporter::SaveTextureToDDS(const char * name) const
{
	ILuint size;
	ILubyte *data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
	if (size > 0) {
		data = new ILubyte[size]; // allocate data buffer
		if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
		{			
			App->fs->SaveDataToLibrary((char*)data, size, name, "Textures", TEXTURE_EXTENSION);
		}

		delete[] data;
	}
}



//Save and Load from own formats--------------------------------------------------
bool ModuleImporter::SaveMeshToOwnFormat(const char* name, uint num_vert, uint num_ind, const float* vert, uint* ind, const float* normals, const float* texture_coords)const
{
	bool ret = true;

	//DATA ORDER: tag - num vertex - num index - vertex - index - has normals - has text coords - normals - text coords

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

	App->fs->SaveDataToLibrary(data,size, name, "Meshes", FORMAT_EXTENSION);

	delete[] data;

	return ret;
}

void ModuleImporter::LoadMeshFromOwnFormat(const char * name, GameObject* obj) const
{
	char* data = nullptr;

	if (App->fs->LoadDataFromLibrary(&data, name, "Meshes", FORMAT_EXTENSION) == false)
	{
		return;
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
		return;
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
	ResourceMesh* resource_mesh = (ResourceMesh*)App->resource_m->CreateResource(Resource::MESH);
	resource_mesh->SetData(vert, ind, num_vert, num_ind, normals, texture_coord);
	obj->CreateComponent_Mesh(name, resource_mesh);
}

bool ModuleImporter::SaveSceneToOwnFormat(const char* name)
{
	//DATA ORDER: tag - num objects - [object]

	bool ret = true;

	uint size = sizeof(uint) * 2;

	std::list<std::pair<char*, uint>> object_buffers;
	std::vector<GameObject*> objects_to_save = App->scene->root->GetChildrens();


	//Saves all object data to buffers
	for (int i = 0; i < objects_to_save.size(); i++)
	{
		size += SaveGameObjectToOwnFormat(object_buffers, objects_to_save[i]);
	}
	

	//Save tag
	char* data = new char[size];
	char* cursor = data;
	uint size_of = sizeof(uint);
	uint tag = SCENE_SAVETAG;

	memcpy(cursor, &tag, size_of);
	cursor += size_of;

	//Save num objs
	uint num_objs = object_buffers.size();
	memcpy(cursor, &num_objs, size_of);
	cursor += size_of;


	//Stores all object data
	for (std::list<std::pair<char*, uint>>::iterator it = object_buffers.begin(); it != object_buffers.end(); it++)
	{	
		size_of = (*it).second;
		memcpy(cursor, (*it).first, size_of);
		cursor += size_of;

		delete[] (*it).first;
	}
	

	App->fs->SaveDataToLibrary(data, size, name, "Scenes", FORMAT_EXTENSION);



	delete[] data;
	LOG("Saved Scene %s to Library", name);

	return ret;
}


uint ModuleImporter::SaveGameObjectToOwnFormat(std::list<std::pair<char*, uint>> &buffer, GameObject* to_save)
{
	//DATA ORDER: UID of obj - size of name - name -
				// UID of parent
			    //  UID of transform - pos - scale - rot 
				//  num of meshes - [UID of mesh - mesh name size - mesh name]
				//  UID of material - size of texture name - texture name
				//UID of camera - near dist - far dist - active

	//STORE SIZE-------------------------------------------------------------------
	uint size = 1;

	std::vector<Component*> meshes = to_save->GetAllComponentOfType(COMPONENT_MESH);


	size += sizeof(uint);
	std::string name = to_save->GetName();
	size += sizeof(uint);
	size += name.size();

	//UID parent
	size += sizeof(uint);

	//Transform
	CompTransform* transform = ((CompTransform*)to_save->GetComponentByType(COMPONENT_TRANSFORM));	
	size += sizeof(uint);
	if (transform != nullptr)
	{		
		size += transform->PrepareToSave();
	}	

	//Meshes
	size += sizeof(uint); //num of meshes
	for (int i = 0; i < meshes.size(); i++)
	{
		size += meshes[i]->PrepareToSave();
	}

	//Material
	size += sizeof(uint);
	ComponentMaterial* mat = (ComponentMaterial*)to_save->GetComponentByType(COMPONENT_MATERIAL);	
	if (mat != nullptr)
	{
		size += mat->PrepareToSave();
	}

	//Camera
	size += sizeof(uint);
	ComponentCamera* cam = (ComponentCamera*)to_save->GetComponentByType(COMPONENT_CAMERA);
	if (cam != nullptr)
	{
		size += cam->PrepareToSave();
	}
	
	//COPY DATA------------------------------------------------------
	char* data = new char[size];
	char* cursor = data;
	uint size_of = 0;


	//Copy UID
	size_of = sizeof(uint);
	uint objID = to_save->GetID();
	memcpy(cursor, &objID , size_of);
	cursor += size_of;

	//Copy name size
	size_of = sizeof(uint);
	uint name_size[] = { -1 };
	name_size[0] = name.size();
	memcpy(cursor, name_size, size_of);
	cursor += size_of;

	//Copy name
	size_of = name_size[0];
	memcpy(cursor, name.data(), size_of);
	cursor += size_of;

	//Copy UID of parent
	uint parentID = to_save->GetParent()->GetID();
	size_of = sizeof(uint);
	memcpy(cursor, &parentID, size_of);
	cursor += size_of;

	//Copy UID of transform
	
	uint transformID = 0;
	if (transform != nullptr)
		transformID = transform->GetID();
	size_of = sizeof(uint);
	memcpy(cursor, &transformID, size_of);
	cursor += size_of;

	//Copy transform
	if (transform != nullptr)
	{
		transform->Save(cursor);
	}



	//Copy meshes
	uint num_of_meshes = meshes.size();
	size_of = sizeof(uint);
	memcpy(cursor, &num_of_meshes, size_of);
	cursor += size_of;

	for (int i = 0; i < num_of_meshes; i++)
	{
		meshes[i]->Save(cursor);
	}

	
	//copy material & texture
	uint matID = 0;	
	if(mat != nullptr)
		matID = mat->GetID();

	size_of = sizeof(uint);
	memcpy(cursor, &matID, size_of);
	cursor += size_of;

	if (mat != nullptr)
	{
		mat->Save(cursor);
	}

	//copy cam settings
	uint camID = 0;
	if (cam != nullptr)
		camID = cam->GetID();

	size_of = sizeof(uint);
	memcpy(cursor, &camID, size_of);
	cursor += size_of;

	if (cam != nullptr)
	{
		cam->Save(cursor);
	}
		

	data[size-1] = '\0';
	std::pair<char*, uint> pair_of_data;
	buffer.push_back(pair_of_data);
	buffer.back().first = data;
	buffer.back().second = size;

	//Load children 
	std::vector<GameObject*> children = to_save->GetChildrens();
	for (int i = 0; i < children.size(); i++)
	{
		size += SaveGameObjectToOwnFormat(buffer, children[i]);
	}
	
	return size;
}



GameObject * ModuleImporter::LoadSceneFromOwnFormat(const char * name)
{

	char* data = nullptr;

	if (App->fs->LoadDataFromLibrary(&data, name, "Scenes", FORMAT_EXTENSION) == false)
	{
		return nullptr;
	}

	CleanTemps();

	//Clean Current Scene
	App->scene->CleanScene();
	
	//Get data from buffer---------------
	//DATA ORDER: tag - num objects - [object]

	//Get tag and check that its a scene
	char* cursor = data;
	uint tag[] = { -1 };
	uint size_of = sizeof(uint);
	memcpy(tag, cursor, size_of);

	if (*tag != SCENE_SAVETAG)
	{
		LOG("ERROR: this is not a scene");
		return nullptr;
	}
	cursor += size_of;
	
	//Load Num of OBJ
	uint num_objects[] = { -1 };
	size_of = sizeof(uint);
	memcpy(num_objects, cursor, size_of);
	cursor += size_of;

	//Load all obj 
	for (int i = 0; i < num_objects[0]; i++)
	{
		LoadObjectFromOwnFormat(cursor);
	}

	//Set all parents properly
	std::vector<GameObject*> objects_in_scene = App->scene->root->GetChildrens();
	for (int i = 0; i < objects_in_scene.size(); i++)
	{
		if (tmp_parent_ids[i] == 0)
			continue;


		for (int j = 0; j < objects_in_scene.size(); j++)
		{
			if (objects_in_scene[j] == objects_in_scene[i])
				continue;

			GameObject* parent = objects_in_scene[j]->FindChildByID(tmp_parent_ids[i]);

			if (parent != nullptr)
			{
				objects_in_scene[i]->SetParent(parent);
				break;
			}
		}
	}


	delete[] data;
	LOG("Loading scene %s", name);	

	return nullptr;
}

uint ModuleImporter::LoadObjectFromOwnFormat(char*& cursor)
{
	//DATA ORDER: UID of obj - size of name - name -
	// UID of parent
	//  UID of transform - pos - scale - rot 
	//  num of meshes - [UID of mesh - mesh name size - mesh name]
	//  UID of material - size of texture name - texture name
	// UID of camera - near distance - far distance
	uint object_id = 0 ;

	uint size_of = sizeof(uint);
	memcpy(&object_id, cursor, size_of);
	cursor += size_of;

	uint size_of_name;
	size_of = sizeof(uint);
	memcpy(&size_of_name, cursor, size_of);
	cursor += size_of;

	char* obj_name = new char[size_of_name + 1];

	size_of = size_of_name;
	memcpy(obj_name, cursor, size_of);
	cursor += size_of;
	obj_name[size_of_name] = '\0';


	GameObject* new_obj = App->scene->CreateGameObject(obj_name, App->scene->root, object_id);

	delete[] obj_name;

	uint parent_id = 0 ;
	size_of = sizeof(uint);
	memcpy(&parent_id, cursor, size_of);
	cursor += size_of;
	tmp_parent_ids.push_back(parent_id);


	uint transform_id =  0 ;
	size_of = sizeof(uint);
	memcpy(&transform_id, cursor, size_of);
	cursor += size_of;


	if (transform_id != 0)
	{
		float pos[] = { 0, 0, 0 };
		size_of = sizeof(float) * 3;
		memcpy(pos, cursor, size_of);
		cursor += size_of;

		float scale[] = { 1, 1, 1 };
		size_of = sizeof(float) * 3;
		memcpy(scale, cursor, size_of);
		cursor += size_of;

		float rot[] = { 0, 0, 0 , 1 };
		size_of = sizeof(float) * 4;
		memcpy(rot, cursor, size_of);
		cursor += size_of;

		new_obj->CreateComponent_Transform(float3(pos[0], pos[1], pos[2]), float3(scale[0], scale[1], scale[2]), Quat(rot[0], rot[1], rot[2], rot[3]), transform_id);
	}
	

	uint num_meshes = 0;
	size_of = sizeof(uint);
	memcpy(&num_meshes, cursor, size_of);
	cursor += size_of;

	//[UID of mesh - mesh name size - mesh name]
	for(int i=0;i<num_meshes;i++)
	{
		uint mesh_id = 0;
		size_of = sizeof(uint);
		memcpy(&mesh_id, cursor, size_of);
		cursor += size_of;

		uint mesh_name_size = 0;
		size_of = sizeof(uint);
		memcpy(&mesh_name_size, cursor, size_of);
		cursor += size_of;

		char* mesh_name = new char[mesh_name_size + 1];

		size_of = mesh_name_size;
		memcpy(mesh_name, cursor, size_of);
		cursor += size_of;

		mesh_name[mesh_name_size] = '\0';

		LoadMeshFromOwnFormat(mesh_name, new_obj);
		delete[] mesh_name;
	}
	
	//UID of material - size of texture name - texture name
	uint mat_id = 0;
	size_of = sizeof(uint);
	memcpy(&mat_id, cursor, size_of);
	cursor += size_of;

	if (mat_id != 0)
	{
		uint text_name_size = 0;
		size_of = sizeof(uint);
		memcpy(&text_name_size, cursor, size_of);
		cursor += size_of;

		char* text_name = new char[text_name_size + 1];

		size_of = text_name_size;
		memcpy(text_name, cursor, size_of);
		cursor += size_of;

		text_name[text_name_size] = '\0';

		bool loaded = false;
		uint texture_ID = -1;
		//Check if texture already loaded and, if not, load it
		for (std::vector<std::pair<std::string, int>>::iterator it = loaded_textures.begin(); it != loaded_textures.end(); it++)
		{
			if ((*it).first.compare(text_name) == 0)
			{
				loaded = true;
				texture_ID = (*it).second;
				break;
			}
		}

		if (loaded == false)
		{
			//Construct path to texture
			std::string path;
			#if _DEBUG
			path = "../Library/";
			#else
			path = "Library/";
			#endif

			path += "Textures/";
			path += text_name;
			path += ".dds";

			texture_ID = LoadTexture(path.c_str(), true);

			//Save the texture in the already-loaded vector
			std::pair<std::string, int> text;
			text.first = text_name;
			text.second = texture_ID;
			loaded_textures.push_back(text);
		}
		


		ResourceTexture* t = (ResourceTexture*)App->resource_m->CreateResource(Resource::TEXTURE);
		t->SetData(texture_ID, std::experimental::filesystem::path(text_name).stem().string().c_str());
		new_obj->CreateComponent_Material(t, mat_id);

		delete[] text_name;
	}

	//UID of camera - near dist - far dist
	uint cam_id = 0;
	size_of = sizeof(uint);
	memcpy(&cam_id, cursor, size_of);
	cursor += size_of;

	if (cam_id != 0)
	{
		float n_dist = 0, f_dist = 0;
		bool active = false;

		size_of = sizeof(float);

		memcpy(&n_dist, cursor, size_of);
		cursor += size_of;
		memcpy(&f_dist, cursor, size_of);
		cursor += size_of;
		size_of = sizeof(bool);
		memcpy(&active, cursor, size_of);
		cursor += size_of;

		new_obj->CreateComponent_Camera(n_dist, f_dist, active ,cam_id);
	}

	cursor++;
	return 0;
}




