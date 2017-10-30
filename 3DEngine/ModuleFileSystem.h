#pragma once

#include "Module.h"
#include "ComponentMesh.h"

class ModuleFileSystem :public Module
{
public:
	ModuleFileSystem(bool start_enabled = true);
	~ModuleFileSystem();

	bool SaveDataToLibrary(const char* data, uint size ,const char* name, const char* directory, const char* extension) const ;
	bool LoadDataFromLibrary(char ** data, const char * name, const char * directory, const char * extension) const;
	std::string CreateDirectoryInLibrary(const char* folder) const;
};

