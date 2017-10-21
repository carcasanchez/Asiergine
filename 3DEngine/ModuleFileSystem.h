#pragma once

#include "Module.h"
#include "ComponentMesh.h"

class ModuleFileSystem :public Module
{
public:
	ModuleFileSystem(bool start_enabled = true);
	~ModuleFileSystem();


	std::string CreateDirectoryInLibrary(const char* folder);
	
	bool SaveSceneToOwnFormat();

	bool SaveGameObjectToOwnFormat(const char* name, float3 pos, float3 scale, math::Quat rot, std::vector<std::string> childs, std::vector<std::string> meshes, const char* material);
	bool SaveMeshToOwnFormat(const char* name, uint , uint , const float* vert, uint* ind, const float* normals = nullptr, const float* texture_coords=nullptr);
	ComponentMesh* LoadMeshFromOwnFormat(const char* path);

private:
	

};

