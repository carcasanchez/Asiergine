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
		GameObject* obj = SearchNode(scene->mRootNode, scene);

		//Extract name from fbx
		std::string file_path = path ;
		std::string obj_name;
		while (file_path.back() != '\\')
		{
			obj_name.push_back(file_path.back());
			file_path.pop_back();
		}
		std::reverse(obj_name.begin(), obj_name.end());
		while (obj_name.back() != '.')
		{
			obj_name.pop_back();
		}
		obj_name.pop_back();
		//-------------------------------------

		obj->SetName(obj_name.c_str());
		aiReleaseImport(scene);		
	}
	else
	{
		LOG("Error loading scene %s", path);
		ret = false;
	}

	return ret;
}

//Searches every FBX node for data and loades one GameObject per node
GameObject* ModuleImporter::SearchNode(const aiNode* n, const aiScene* scene, GameObject* parent)
{
	GameObject* obj = LoadNewObject(n, parent);

	//Loads all meshes of the node	
	for (int i = 0; i < n->mNumMeshes; i++)
	{
		aiMesh* m = scene->mMeshes[n->mMeshes[i]];
		LoadGeometry(m, obj);
		obj->CreateComponent_Material(texture_id);
	}

	//Searches for children nodes
	for (int i = 0; i < n->mNumChildren; i++)
		SearchNode(n->mChildren[i], scene, obj);


	return obj;
}

//Creates new object and loads transform
GameObject * ModuleImporter::LoadNewObject(const aiNode * n, GameObject* parent)
{	
	GameObject* new_obj = App->scene->CreateGameObject(n->mName.C_Str(), parent);
	
	aiVector3D location;
	aiVector3D scale;
	aiQuaternion rotation;

	//Decompose transformation matrix
	n->mTransformation.Decompose(scale, rotation, location);

	new_obj->CreateComponent_Transform(float3(location.x, location.y, location.z), float3(scale.x, scale.y, scale.z), Quat(rotation.x, rotation.y, rotation.z, rotation.w));

	return new_obj;
}

//Loads meshes from FBX node
bool ModuleImporter::LoadGeometry(const aiMesh* m, GameObject* obj)
{
	bool ret = true;
	
	//Data to fill------------------------------
	float* vertices = nullptr;
	float* normals = nullptr;
	uint* indices = nullptr;
	float* texture_coords = nullptr;


	int numVertx = m->mNumVertices; 
	int numInd = m->mNumFaces * 3;
	//------------------------------------------
	
	//Copy vertices
	vertices = new float[numVertx * 3];
	memcpy(vertices, m->mVertices, sizeof(float) * numVertx * 3);
			
			
	//Copy indices
	 indices = new uint[numInd * 3];	
	if(m->HasFaces())
	{
		for (int k = 0; k < m->mNumFaces; k++)
		{
			if (m->mFaces[k].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
				memcpy(&indices[k * 3], m->mFaces[k].mIndices, 3 * sizeof(uint));
		}
	}
			

	//Copy normals
	if (m->HasNormals())
	{
		normals = new float[numVertx * 3];
		memcpy(normals, m->mNormals, sizeof(float) * numVertx * 3);
	}	
	else LOG("WARNING: Loading Geometry without normals");


	//copy texture coords
	if (m->HasTextureCoords(0))
	{
		texture_coords = new float[numVertx * 2];
		for (int k = 0; k < numVertx; ++k) 
		{
			texture_coords[k * 2] = m->mTextureCoords[0][k].x;
			texture_coords[k * 2 + 1] = m->mTextureCoords[0][k].y;
		}
	}
	else LOG("WARNING: Loading Geometry without texture coords");

	//If everything goes OK, create a new Mesh
	if (ret)
	{
		obj->CreateComponent_Mesh(vertices, indices, numVertx, numInd, normals, 0, texture_coords);
		delete[] texture_coords;
	}

	return ret;
}

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

