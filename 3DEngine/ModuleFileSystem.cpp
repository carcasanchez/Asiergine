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
		LOG("DevIL version is different!\n");
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


GLuint ModuleFileSystem::LoadTexture(const char * path)
{
	//Gen image
	ILuint img_id = 0;
	ilGenImages(1, &img_id);
	ilBindImage(img_id);

	//load from path
	ilLoadImage(path);



	ILuint devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		LOG("Devil Error (ilInit: %s)", iluErrorString(devilError));	
	}

	// If the image is flipped
	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}

	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	
	

	devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		LOG("Devil Error (ilInit: %s)", iluErrorString(devilError));
	}

	//Send texture to GPU
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &img_id);
	glBindTexture(GL_TEXTURE_2D, img_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
		0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	
	devilError = ilGetError();
	if (devilError != IL_NO_ERROR)
	{
		LOG("Devil Error (ilInit: %s)", iluErrorString(devilError));
	}

	//Store the ID in file system
	textureIDs.push_back(img_id);
	return img_id;
}
