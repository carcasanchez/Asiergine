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
	iluInit();	ilutRenderer(ILUT_OPENGL);	ILuint devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		printf("Devil Error (ilInit: %s\n", iluErrorString(devilError));
		exit(2);
	}
	
	return ret;
}

bool ModuleFileSystem::Start()
{
	bool ret = true;


	return ret;
}

bool ModuleFileSystem::CleanUp()
{
	//Release geometries
	for (std::vector<Geometry*>::iterator it = geometries.begin(); it != geometries.end(); it++)
	{
		if ((*it) != nullptr)
			delete (*it);
	}

	// detach log stream
	aiDetachAllLogStreams();

	return true;
}


//Loads data from a given path
bool ModuleFileSystem::LoadGeometry(const char * path)
{
/*	//--indice elements
	float vertex[24] =
	{
		20.0, 20.0,  20.0,
		20.0, 20.0,  10.0,
		10.0, 20.0,  20.0,
		10.0, 20.0,  10.0,

		20.0, 10.0, 20.0,
		20.0, 10.0, 10.0,
		10.0,  10.0, 20.0,
		10.0, 10.0, 10.0,
	};

	uint index[36] =
	{ 1, 0, 2,  3, 1, 2,
		3, 2, 6,  7, 3, 6,
		3, 7, 5,  1, 3, 5,
		7, 6, 4,  5, 7, 4,
		5, 4, 0,  1, 5, 0,
		0, 4, 6,  2, 0, 6,
	};*/

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		
		for (int i = 0, j = scene->mNumMeshes; i < j; i++)
		{
			int numVertx = scene->mMeshes[i]->mNumVertices;
			int numInd = scene->mMeshes[i]->mNumFaces*3;

			//Copy vertex
			float* vertices = new float[numVertx * 3];
			memcpy(vertices, scene->mMeshes[i]->mVertices, sizeof(float) * numVertx * 3);
			LOG("New mesh with %d vertices", numVertx);
			
			//Copy indices
			uint* indices = new uint[numInd * 3];
	
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

			Geometry* new_geom = new Geometry(vertices, indices, numVertx, numInd);


			//Copy normals
			if (scene->mMeshes[i]->HasNormals())
			{
				new_geom->normals = new float[numVertx * 3];
				memcpy(new_geom->normals, scene->mMeshes[i]->mNormals, sizeof(float) * numVertx * 3);
			}

		
			geometries.push_back(new_geom);
			LOG("New mesh created from %s", path);

		}		
		
		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene %s", path);

	
	return true;
}


Texture* ModuleFileSystem::LoadTexture(const char * path)
{
	//Gen image
	ILuint ImgId = 0;
	ilGenImages(1, &ImgId);
	ilBindImage(ImgId);

	//load from path
	ilLoadImage(path);

	//Allocate memory
	float width = ilGetInteger(IL_IMAGE_WIDTH);
	float height = ilGetInteger(IL_IMAGE_HEIGHT);
	GLubyte* pixmap = new GLubyte[width * height * 3];
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGB,
		IL_UNSIGNED_BYTE, pixmap);

	Texture* new_text = new Texture(width, height, pixmap);

	ilBindImage(0);
	ilDeleteImage(ImgId);

	return new_text;
}
