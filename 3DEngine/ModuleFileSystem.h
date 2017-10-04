#pragma once
#include "Module.h"
#include "Geometry.h"
#include "Texture.h"


class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem();
	ModuleFileSystem(bool start_enabled);
	~ModuleFileSystem();

	bool Init(const JSON_Object* config_data);
	bool Start();
	bool CleanUp();

	bool LoadGeometry(const char* path);
	Texture* LoadTexture(const char * path);


	std::vector<Geometry*> geometries;

private:

};
