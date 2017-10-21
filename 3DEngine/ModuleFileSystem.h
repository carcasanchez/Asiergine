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

	bool SaveMeshToOwnFormat(const char* name, uint , uint , const float* vert, uint* ind, const float* normals = nullptr, const float* texture_coords=nullptr);
	ComponentMesh* LoadMeshFromOwnFormat(const char* path);

private:
	

};

