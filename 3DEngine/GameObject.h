#pragma once
#include "Globals.h"

class Component;
class CompTransform;

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

	const GameObject* GetParent() { return parent; }
	std::vector<GameObject*> GetChildrens() { return children;}
	void SetParent(GameObject* new_parent);

	Component* CreateComponent_Transform(float3 trans = float3(0, 0, 0), float3 scaling = float3(1, 1, 1), Quat rot = Quat::identity);

private:

	GameObject* parent = nullptr;
	std::string name;

	std::vector<Component*> components;
	std::vector<GameObject*> children;

};