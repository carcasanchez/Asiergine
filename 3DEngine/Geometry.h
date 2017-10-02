#pragma once
#include "Globals.h"

struct Geometry
{
	uint id_vertices = 0;  //id in VRAM
	uint num_vertices = 0;
	uint* vertices = nullptr;

	uint id_indices = 0;  //id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	void DrawGeometry();
	Geometry();
	~Geometry();
};