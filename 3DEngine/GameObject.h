#pragma once
#include "Globals.h"

class Component;
class CompTransform;
class ComponentMesh;
class ComponentMaterial;

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

	const GameObject* GetParent() { return parent; }
	std::vector<GameObject*> GetChildrens() { return children;}
	void SetParent(GameObject* new_parent);

	CompTransform* CreateComponent_Transform(float3 trans = float3(0, 0, 0), float3 scaling = float3(1, 1, 1), Quat rot = Quat::identity);
	ComponentMesh* CreateComponent_Mesh(float* ver, uint* ind, uint num_vert, uint num_ind, uint texture_id = 0, float* texture_coords = nullptr);
	ComponentMaterial* CreateComponent_Material(uint texture_id);


private:

	GameObject* parent = nullptr;
	std::string name;

	std::vector<Component*> components;
	std::vector<GameObject*> children;

};