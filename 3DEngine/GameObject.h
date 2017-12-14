#pragma once
#include "Globals.h"
#include "QuadTree.h"
#include "ComponentAudio.h"

class Component;
class CompTransform;
class ComponentMesh;
class ComponentMaterial;
class ComponentCamera;
class ComponentLight;
class ComponentMovement;

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
	void SetStatic(bool obj_static);
	void SendAllMeshesToDraw();
	void EraseChild(GameObject* game_object);

	CompTransform* CreateComponent_Transform(float3 trans = float3(0, 0, 0), float3 scaling = float3(1, 1, 1), Quat rot = Quat::identity, uint UID = 0);
	ComponentMesh* CreateComponent_Mesh(const char* name, ResourceMesh* m = nullptr, uint UID = 0);
	ComponentMaterial* CreateComponent_Material(ResourceTexture* t = nullptr, uint UID = 0);
	ComponentCamera* CreateComponent_Camera(float near_dist, float far_dist, bool active = false, uint UID = 0);
	ComponentLight* CreateComponent_Light(uint UID = 0);
	ComponentAudio* CreateComponent_Audio(uint UID = 0, AUDIO_TYPE t = AUDIO_TYPE::FX);
	ComponentMovement* CreateComponent_Movement(float3 point1 = float3(0, 0, 0), float3 point2 = float3(0, 0, 0), float speed = 0.001, uint UID = 0);


	void OnEditor();

	bool PutInQuadTree(QuadTreeNodeObj* node);


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

	AABB GetBoundingBox();

	void SetID(uint new_ID)
	{
		UID = new_ID;
	}


	void CheckTriangleCollision(math::LineSegment &l, float& distance, GameObject* &best_candidate);
	GameObject* FindChildByID(uint uid) const;
	void CheckMouseRayCollision(math::LineSegment &l, float& distance, GameObject* &best_candidate);
	
	
	std::vector<GameObject*> children;

private:

	bool obj_static = false;
	GameObject* parent = nullptr;
	std::string name;

	std::vector<Component*> components;

	uint UID = 0;

};