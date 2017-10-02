#pragma once
#include "Module.h"


class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem();
	ModuleFileSystem(bool start_enabled);
	~ModuleFileSystem();

	bool Init(const JSON_Object* config_data);
	bool Start();

private:

};
