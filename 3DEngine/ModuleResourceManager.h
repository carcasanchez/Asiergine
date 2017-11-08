#pragma once
#include "Module.h"
#include <map>
#include "Resource.h"

class ModuleResourceManager :
	public Module
{
public:
	ModuleResourceManager();
	~ModuleResourceManager();

	bool Init(const JSON_Object* config_data);
	bool CleanUp();

	Resource* GetResource(uint id);

	Resource* CreateResource(Resource::RESOURCE_TYPE type);

private:

	std::map<uint, Resource*> resources;

};

