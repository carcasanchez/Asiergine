#pragma once
#include "Globals.h"
#include "QuadTree.h"

class Component;
class CompTransform;
class ComponentMesh;
class ComponentMaterial;
class ComponentCamera;

class ResourceMesh;
class ResourceTexture;

enum ComponentType;

class GameObject
{
public:
	GameObject(const char* name);
	~GameObject();

	void Update(float real_dt, float game_dt);
	void CleanUp();

	Component* GetComponentByType(ComponentType type);
	std::vector<Component*> GetAllComponentOfType(ComponentType type);
	
	const char* GetName() const { return name.c_str(); }
	void SetName(const char* n) { name = n; }

	GameObject* GetParent() const { return parent; }
	std::vector<GameObject*> GetChildrens() const { return children;}
	void SetParent(GameObject* new_parent);
	bool IsStatic() const { return obj_static; }
	void SetStatic(bool obj_static, GameObject* child);
	void SendAllMeshesToDraw();
	void SetBoundingBox(const ResourceMesh* m);

	CompTransform* CreateComponent_Transform(float3 trans = float3(0, 0, 0), float3 scaling = float3(1, 1, 1), Quat rot = Quat::identity, uint UID = 0);
	ComponentMesh* CreateComponent_Mesh(const char* name, ResourceMesh* m = nullptr, uint UID = 0);
	ComponentMaterial* CreateComponent_Material(ResourceTexture* t = nullptr, uint UID = 0);
	ComponentCamera* CreateComponent_Camera(float near_dist, float far_dist, bool active = false, uint UID = 0);

	void OnEditor();

	bool PutInQuadTree(QuadTreeNodeObj* node);

	const AABB* GetBoundingBox() const
	{
		 return &bounding_box;
	}

	const AABB* GetTransformedBox() const
	{
		return &transformed_bounding_box;
	}

	uint GetID() const
	{
		return UID;
	}

	uint GetParentID() const
	{
		if (parent == nullptr)
			return 0;

		return parent->GetID();
	}

	void SetID(uint new_ID)
	{
		UID = new_ID;
	}

	void CheckTriangleCollision(math::LineSegment &l, float& distance, GameObject* &best_candidate);
	GameObject* FindChildByID(uint uid) const;
	void CheckMouseRayCollision(math::LineSegment &l, float& distance, GameObject* &best_candidate);
	

	

	math::AABB bounding_box = math::AABB(float3::zero, float3::zero);
	std::vector<GameObject*> children;

private:

	bool obj_static = false;
	GameObject* parent = nullptr;
	std::string name;

	std::vector<Component*> components;

	
	math::AABB transformed_bounding_box;
	uint UID = 0;


};