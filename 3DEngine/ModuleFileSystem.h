#pragma once

#include "Module.h"
#include "ComponentMesh.h"

class ModuleFileSystem :public Module
{
public:
	ModuleFileSystem(bool start_enabled = true);
	~ModuleFileSystem();


	std::string CreateDirectoryInLibrary(const char* folder) const;
	


	bool SaveMeshToOwnFormat(const char* name, uint , uint , const float* vert, uint* ind, const float* normals = nullptr, const float* texture_coords=nullptr) const;
	void LoadMeshFromOwnFormat(const char* path, GameObject* obj)const;
	void SaveTextureToDDS(const char* data, uint size, const char* name)const;


	//DEPRECATED-----------------------------------------------------------------------------------------------
	bool SaveSceneToOwnFormat(const char* name, std::vector<std::string> objects);
	GameObject*  LoadSceneFromOwnFormat(const char* path);
	bool SaveGameObjectToOwnFormat(const char* name, float3 pos, float3 scale, math::Quat rot, std::vector<std::string> childs, std::vector<std::string> meshes, const char* material);
	GameObject* LoadObjectFromOwnFormat(const char* path);

private:
	

};

