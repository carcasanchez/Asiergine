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

	//Checks if resource is in memory and loads it if not
	Resource* LoadResource(const char* path);
	Resource* CreateResource(Resource::RESOURCE_TYPE type, uint id = 0);
	Resource* GetResource(uint id);
	bool DeleteResource(uint id);

private:


	void ManageFBX(const char* path);
	uint ManageMesh(const char* path);
	uint CreateMeshMeta(const char* path);

	bool CheckTimestamp(const char* file, const char* meta);

	std::map<uint, Resource*> resources;

};

