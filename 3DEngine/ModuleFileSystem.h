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
	





	//DEPRECATED-----------------------------------------------------------------------------------------------
	bool SaveSceneToOwnFormat(const char* name, std::vector<std::string> objects);
	GameObject*  LoadSceneFromOwnFormat(const char* path);
	bool SaveGameObjectToOwnFormat(const char* name, float3 pos, float3 scale, math::Quat rot, std::vector<std::string> childs, std::vector<std::string> meshes, const char* material);
	GameObject* LoadObjectFromOwnFormat(const char* path);

private:
	

};

