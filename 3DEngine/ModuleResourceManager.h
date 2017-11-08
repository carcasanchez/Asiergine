#pragma once
#include "Module.h"
#include <map>

class Resource;

class ModuleResourceManager :
	public Module
{
public:
	ModuleResourceManager();
	~ModuleResourceManager();

	bool Init(const JSON_Object* config_data);

	Resource* GetResource(uint id);

private:

	std::map<uint, Resource*> resources;

};

