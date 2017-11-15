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
	update_status PreUpdate(float real_dt, float game_dt);
	bool CleanUp();

	//Checks if resource is in memory and loads it if not
	Resource* LoadResource(const char* path);
	Resource* CreateResource(Resource::RESOURCE_TYPE type, uint id = 0);
	Resource* GetResource(uint id);
	void SetToDelete(uint id);
	

private:

	bool DeleteResource(uint id);

	void ManageFBX(const char* path);
	uint ManageMesh(const char* path);
	uint ManageTexture(const char* path, const char* extension);


	uint CreateMeshMeta(const char* path);
	uint CreateTextureMeta(const char* path);


	bool CheckTimestamp(const char* file, const char* meta);

	std::map<uint, Resource*> resources;

	std::vector<uint> to_delete;

};

