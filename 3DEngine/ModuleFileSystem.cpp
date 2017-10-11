#include "Globals.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "ModuleFileSystem.h"


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

ModuleFileSystem::ModuleFileSystem()
{

}

ModuleFileSystem::ModuleFileSystem(bool start_enabled) : Module(start_enabled)
{
	name = "file_system";
}

ModuleFileSystem::~ModuleFileSystem()
{
}

bool ModuleFileSystem::Init(const JSON_Object* config_data)
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

bool ModuleFileSystem::Start()
{
	bool ret = true;


	return ret;
}

bool ModuleFileSystem::CleanUp()
{
	UnloadGeometry();

	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

void ModuleFileSystem::LoadFile(const char * path)
{
	std::string tmp = path;
	std::string extension;

	LOG("Loading file from %s", path);
	
	//Get file extension (reversed)
	while (tmp.back() != '.')
	{
		extension.push_back(tmp.back());
		tmp.pop_back();
	}
	//Normalize to lower case
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
	//Reverse to be human readable
	std::reverse(extension.begin(), extension.end());


	//Check if extension is valid
	if (extension.compare("fbx") == 0)
	{
		LoadGeometry(path);
	}
	else if (extension.compare("png") == 0 || extension.compare("jpg") == 0 || extension.compare("dds") == 0)
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
					geometries[i]->texture_id = new_id;
			}
			
		}
		
	}
	else LOG("ERROR: File extension '.%s' not allowed", extension.c_str());

}


//Loads data from a given path
bool ModuleFileSystem::LoadGeometry(const char * path)
{
	bool ret = true;

	Geometry* new_geom = nullptr;
	float* vertices = nullptr;
	float* normals = nullptr;
	uint* indices = nullptr;
	int numVertx = 0, numInd = 0;
	int text_id =0 ;
	float* texture_coords = nullptr;

	UnloadGeometry();

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);


	//Search for textures
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
			
			if(text_id==0)
				LOG("Warning: --------Loading Mesh Without Texture");
		}


	if (scene != nullptr && scene->HasMeshes())
	{

		// Use scene->mNumMeshes to iterate on scene->mMeshes array		
		for (int i = 0, j = scene->mNumMeshes; i < j; i++)
		{
			numVertx = scene->mMeshes[i]->mNumVertices;
			numInd = scene->mMeshes[i]->mNumFaces*3;

			//Copy vertex
			vertices = new float[numVertx * 3];
			memcpy(vertices, scene->mMeshes[i]->mVertices, sizeof(float) * numVertx * 3);
			
			//Copy indices
			 indices = new uint[numInd * 3];	
			if(scene->mMeshes[i]->HasFaces())
			{
				for (int k = 0; k < scene->mMeshes[i]->mNumFaces; k++)
				{
					if (scene->mMeshes[i]->mFaces[k].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&indices[k * 3], scene->mMeshes[i]->mFaces[k].mIndices, 3 * sizeof(uint));
					}
				}
			}
			

			//Copy normals
			if (scene->mMeshes[i]->HasNormals())
			{
				normals = new float[numVertx * 3];
				memcpy(normals, scene->mMeshes[i]->mNormals, sizeof(float) * numVertx * 3);
			}	

			//copy texture coords
			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				texture_coords = new float[numVertx * 2];
				for (int k = 0; k < numVertx; ++k) {

					texture_coords[k * 2] = scene->mMeshes[i]->mTextureCoords[0][k].x;
					texture_coords[k * 2 + 1] = scene->mMeshes[i]->mTextureCoords[0][k].y;

				}
			}


			//If everything goes OK, create a new Mesh
			if (ret)
			{
				new_geom = new Geometry(vertices, indices, numVertx, numInd, text_id, texture_coords);
				new_geom->normals = normals;
				geometries.push_back(new_geom);
				LOG("New mesh with %d vertices", numVertx);

				App->camera->AdaptToGeometry(new_geom);
				delete[] texture_coords;
			}

		}		
		
		
	}
	else
	{
		LOG("Error loading scene %s", path);
		ret = false;
	}


	
	aiReleaseImport(scene);
	return ret;
}


GLuint ModuleFileSystem::LoadTexture(const char * path)
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

void ModuleFileSystem::UnloadGeometry()
{
	//Release geometries
	for (std::vector<Geometry*>::iterator it = geometries.begin(); it != geometries.end(); it++)
	{
		if ((*it) != nullptr)
			delete (*it);
	}
	geometries.clear();	
}
