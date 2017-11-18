#pragma once
#include "Component.h"
#include "Globals.h"
#include "ResourceMesh.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* game_object);
	~ComponentMesh();

	std::string name;

	void Draw();
	void DebugDraw();
	void Update(float real_dt, float game_dt);
	void OnEditor();

	void SetMesh(ResourceMesh* m)
	{
		if(mesh)
			mesh->DecreaseInstancies();
		mesh = m;
		if (mesh)
			mesh->IncreaseInstancies();
	}
		
	int GetNumVertices() const { 	return mesh ? mesh->GetNumVertices():0;	}
	int GetNumIndices() const { return mesh ? mesh->GetNumIndices() : 0; }
	const float* GetVertices() const { return mesh ? mesh->GetVertices() : nullptr; }
	const uint* GetIndices() const { return mesh ? mesh->GetIndices() : nullptr;}
	bool IsActive() const { return active; }
	const AABB* GetTransformedBox() const { return &transformed_bounding_box;}

	//Scene serialization------------------------
	uint PrepareToSave() const;
	void Save(char* &cursor) const;

private:
	bool want_to_change_mesh = false;
	std::string library_meshes_path;
	std::vector<std::string> meshes_to_show;
	ResourceMesh* mesh = nullptr;
	math::AABB transformed_bounding_box;
	
};