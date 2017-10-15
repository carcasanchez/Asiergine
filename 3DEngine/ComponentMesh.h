#pragma once
#include "Component.h"
#include "Globals.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* game_object, float* ver, uint* ind, uint num_vert, uint num_ind, uint texture_id = 0, float* texture_coords = nullptr);
	~ComponentMesh();

	void Draw();
	void DebugDraw();

	void SetNormals(float* n) { normals = n; }

	int GetNumVertices() { return num_vertices; }
	float* GetVertices() { return vertices; }

private:
	uint id_vertices = 0;  //id in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_indices = 0;  //id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	float* normals = nullptr;

	uint texture_id = 0;
	uint text_coord_id = 0;
};