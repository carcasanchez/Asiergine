#pragma once
#include "Globals.h"

struct Geometry
{
	Geometry(float* ver, uint* ind, uint num_vert, uint num_ind, uint texture_id = 0, float* texture_coords = nullptr);
	~Geometry();

	void Draw();
	void DebugDraw();

	uint id_vertices = 0;  //id in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_indices = 0;  //id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	float* normals = nullptr;

	uint texture_id = 0;

	
};