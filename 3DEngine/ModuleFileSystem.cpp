#include "Globals.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "ModuleFileSystem.h"

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

	return true;
}


//Loads data from a given path
bool ModuleFileSystem::LoadGeometry(const char * path)
{
	//TODO: load geom from path

	//--indice elements
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
	};


	Geometry* new_geom;
	new_geom = new Geometry(vertex, index, 24, 36);
	
	geometries.push_back(new_geom);


	return true;
}
