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


//Loads data from a given path
bool ModuleFileSystem::LoadGeometry(const char * path)
{


	return false;
}
