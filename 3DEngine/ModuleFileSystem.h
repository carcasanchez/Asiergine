#pragma once

#include "Module.h"

class ModuleFileSystem :public Module
{
public:
	ModuleFileSystem(bool start_enabled = true);
	~ModuleFileSystem();

	bool SaveMeshToOwnFormat(uint , uint , float* vert, uint* ind, float* normals = nullptr, float* texture_coords=nullptr);
};

