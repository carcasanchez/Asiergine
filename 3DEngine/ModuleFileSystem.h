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


	bool SaveDataTo(const char* data, uint size, const char* path) const;
	uint LoadDataFrom(char* &data, const char * path) const;
	std::string CreateDirectoryInLibrary(const char* folder) const;

	//Creates directory or returns its path if it exists
	std::string CreateDirectoryInAssets(const char* folder) const;
	bool ExistsFile(const char* path) const;

	bool CloneFile(const char* source, const char* destination)const;


	//Deprecated----------------------------------
	bool SaveDataToLibrary(const char* data, uint size ,const char* name, const char* directory, const char* extension) const ;
	bool SaveDataToAssets(const char* data, uint size, const char* name, const char* extension) const;
	bool LoadDataFromLibrary(char ** data, const char * name, const char * directory, const char * extension) const;

};

