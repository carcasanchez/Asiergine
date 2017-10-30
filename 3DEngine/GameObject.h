#pragma once
#include "Globals.h"
#include "QuadTree.h"

class Component;
class CompTransform;
class ComponentMesh;
class ComponentMaterial;
class ComponentCamera;

enum ComponentType;

class GameObject
{
public:
	GameObject(const char* name);
	~GameObject();

	void Update();
	void CleanUp();

	Component* GetComponentByType(ComponentType type);
	
	const char* GetName() const { return name.c_str(); }
	void SetName(const char* n) { name = n; }

	GameObject* GetParent() { return parent; }
	std::vector<GameObject*> GetChildrens() { return children;}
	void SetParent(GameObject* new_parent);
	bool IsStatic() { return obj_static; }

	CompTransform* CreateComponent_Transform(float3 trans = float3(0, 0, 0), float3 scaling = float3(1, 1, 1), Quat rot = Quat::identity);
	ComponentMesh* CreateComponent_Mesh( float* ver, uint* ind, uint num_vert, uint num_ind, float* normals, float* texture_coords = nullptr);
	ComponentMaterial* CreateComponent_Material(uint texture_id);
	ComponentCamera* CreateComponent_Camera(float , float, bool active = false);

	void OnEditor();

	bool PutInQuadTree(QuadTreeNodeObj* node);

	const AABB* GetBoundingBox()
	{
		 return &bounding_box;
	}

	const AABB* GetTransformedBox()
	{
		return &transformed_bounding_box;
	}

	int16_t GetID()
	{
		return UID;
	}


	math::AABB bounding_box = math::AABB(float3::zero, float3::zero);

private:
	bool obj_static = false;
	GameObject* parent = nullptr;
	std::string name;

	std::vector<Component*> components;
	std::vector<GameObject*> children;
	
	math::AABB transformed_bounding_box;

	int16_t UID = 0;

};