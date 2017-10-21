#include "Globals.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "ModuleImporter.h"
#include "GameObject.h"
#include "ComponentMesh.h"

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
/*	else if (extension.compare("png") == 0 || extension.compare("jpg") == 0 || extension.compare("dds") == 0)
	{
		if (geometries.empty())
		{
			LOG("WARNING: Scene has no geometries. Could not load texture.")
		}
		else
		{	
			//Loads texture and puts it in all geometry
			int new_id = LoadTexture(path);

			if (new_id != 0)			{
				glDeleteTextures(1, &geometries[0]->texture_id);
				for (int i = 0; i < geometries.size(); i++)
				{
					geometries[i]->texture_id = new_id;
					if (geometries[i]->text_coord_id == 0)
					{
						LOG("WARNING: Geometry without texture coords");
					}
				}

				first_texture_id = new_id;
			}
			
		}
		
	}*/
	else LOG("ERROR: File extension '.%s' not allowed", extension.c_str());

}


//Load FBX scene----------------------------------
bool ModuleImporter::LoadFBX(const char * path)
{
	bool ret = true;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	
	if (scene != nullptr && scene->HasMeshes())
	{
		//Extract name from fbx
		std::string file_path = path ;
		std::string scene_name;
		while (file_path.back() != '\\')
		{
			scene_name.push_back(file_path.back());
			file_path.pop_back();
		}
		std::reverse(scene_name.begin(), scene_name.end());
		while (scene_name.back() != '.')
		{
			scene_name.pop_back();
		}
		scene_name.pop_back();

		ImportScene(scene, scene_name.c_str());


		aiReleaseImport(scene);		
	}
	else
	{
		LOG("Error loading scene %s", path);
		ret = false;
	}

	return ret;
}

std::string ModuleImporter::ImportScene(const aiScene * scene, const char* name)
{
	std::vector<std::string> objects_in_scene;

	for (int i = 0; i < scene->mRootNode->mNumChildren; i++)
		objects_in_scene.push_back(SearchNode(scene->mRootNode->mChildren[i], scene));

	App->fs->SaveSceneToOwnFormat(name, objects_in_scene);

	return std::string();
}

//Searches every FBX node for data and imports one GameObject per node
std::string ModuleImporter::SearchNode(const aiNode* n, const aiScene* scene)
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

	mesh_id = 0;

	std::vector<std::string> meshes;
	std::vector<std::string> children;
	std::string material_name;

	//Last mesh material
	uint material_index = 0;

	//Loads all meshes of the node	
	for (int i = 0; i < n->mNumMeshes; i++)
	{
		aiMesh* m = scene->mMeshes[n->mMeshes[i]];
		meshes.push_back(ImportGeometry(m, n->mName.C_Str()));
		material_index = m->mMaterialIndex;
	}

	//Searches for children nodes
	for (int i = 0; i < n->mNumChildren; i++)
		children.push_back(SearchNode(n->mChildren[i], scene));

	
	if (scene->HasMaterials() && material_index != 0)
	{
		scene->mMaterials[material_index]->Get(AI_MATKEY_NAME, material_name);
	}

	//Saves object to .carca
	App->fs->SaveGameObjectToOwnFormat(n->mName.C_Str(), location, scale, rot, children, meshes, material_name.c_str());

	return n->mName.C_Str();
}


//Loads meshes from FBX node
std::string ModuleImporter::ImportGeometry(const aiMesh* m, const char* obj_name)
{	
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
		App->fs->SaveMeshToOwnFormat(name.c_str(), numVertx, numInd, vertices.data(), indices.data(), normals.data(), texture_coords.data());
	

	return name;
}


//////////////////DEPRECATED----------------------------------------------
//Get the very first texture of the FBX (called once)
int ModuleImporter::SearchForTexture(const aiScene* scene, const char* path)
{
	int text_id = 0;	
	
	if (scene->HasMaterials())
		if (scene->mMaterials[0]->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString s;
			scene->mMaterials[0]->GetTexture(aiTextureType_DIFFUSE, 0, &s);
			std::string  texture_name = s.C_Str();
			std::string  geom_path = path;

			//Construc the general path for the texture
			while (geom_path.back() != '\\')
			{
				geom_path.pop_back();
			}
			geom_path += texture_name;
			text_id = LoadTexture(geom_path.c_str());

			if (text_id == 0)
				LOG("Warning: --------Scene missing textures");
		}

	return text_id;
}

//Load texture from image-----------------------------
GLuint ModuleImporter::LoadTexture(const char * path)
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
//////////////////
