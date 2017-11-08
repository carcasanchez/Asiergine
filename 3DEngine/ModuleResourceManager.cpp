#include "Globals.h"
#include "Application.h"
#include "ModuleResourceManager.h"

#include "mmgr\mmgr.h"
#include "Resource.h"

ModuleResourceManager::ModuleResourceManager()
{
	name = "resource manager";
}


ModuleResourceManager::~ModuleResourceManager()
{
}


bool ModuleResourceManager::Init(const JSON_Object* config_data)
{
	bool ret = true;

	return ret;
}

Resource * ModuleResourceManager::GetResource(uint id)
{
	Resource* res = nullptr;
	if(resources.count(id) >0)
			res = resources[id];

	return res;
}
