#pragma once
#include "Resource.h"
class ResourceMesh :
	public Resource
{
public:
	ResourceMesh(uint id);
	~ResourceMesh();

	void SetData(float* ver, uint* ind, uint num_vert, uint num_ind, float* normals, float* texture_coords);
	
	void Draw(const float* transform, uint texture_id) const;

	int GetNumVertices() const { return num_vertices; }
	int GetNumIndices() const { return num_indices; }
	const float* GetVertices() const { return vertices; }
	const uint* GetIndices() const { return indices; }

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

