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

	void CheckFileStatus(const char* path);


private:

	void LoadFile(const char* path);

	std::map<uint, Resource*> resources;

};

