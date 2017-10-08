#pragma once
#include "Module.h"
#include "Geometry.h"


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
	GLuint LoadTexture(const char * path);

	void UnloadGeometry();


	std::vector<Geometry*> geometries;

private:

};
