#pragma once

#include "Module.h"
#include "ComponentMesh.h"

class ModuleFileSystem :public Module
{
public:
	ModuleFileSystem(bool start_enabled = true);
	~ModuleFileSystem();

	std::string  GetLibraryDirectory() const;
	std::string  GetAssetDirectory() const;

	bool SaveDataToLibrary(const char* data, uint size ,const char* name, const char* directory, const char* extension) const ;
	bool LoadDataFromLibrary(char ** data, const char * name, const char * directory, const char * extension) const;
	std::string CreateDirectoryInLibrary(const char* folder) const;


	bool ExistsFile(const char* path) const;
};

