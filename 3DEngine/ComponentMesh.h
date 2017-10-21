#pragma once
#include "Component.h"
#include "Globals.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* game_object, float* ver, uint* ind, uint num_vert, uint num_ind, float* normals, float* texture_coords = nullptr);
	~ComponentMesh();

	std::string name;

	void Draw();
	void DebugDraw();
	void Update();
	void OnEditor();

	void SetNormals(float* n) { normals = n; }

	int GetNumVertices() { return num_vertices; }
	const float* GetVertices() { return vertices; }

	bool IsActive() { return active; }

private:


	uint id_vertices = 0;  //id in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_indices = 0;  //id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	float* normals = nullptr;

	uint text_coord_id = 0;
	float* texture_coords = nullptr;

};