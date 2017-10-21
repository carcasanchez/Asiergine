#pragma once

#include "Module.h"
#include "ComponentMesh.h"

class ModuleFileSystem :public Module
{
public:
	ModuleFileSystem(bool start_enabled = true);
	~ModuleFileSystem();


	std::string CreateDirectoryInLibrary(const char* folder);

	bool SaveMeshToOwnFormat(const char* name, uint , uint , float* vert, uint* ind, float* normals = nullptr, float* texture_coords=nullptr);
	ComponentMesh* LoadMeshFromOwnFormat(const char* path);

private:
	

};

