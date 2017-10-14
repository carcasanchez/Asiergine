#pragma once
#include <string>
#include <vector>

class Component;

enum ComponentType;

class GameObject
{
public:
	GameObject(const char* name);
	~GameObject();

	void Update();

	Component* GetComponentByType(ComponentType type);
	
	const char* GetName() const { return name.c_str(); }

	const GameObject* GetParent() { return parent; }
	void SetParent(GameObject*);

private:

	GameObject* parent = nullptr;
	std::string name;

	std::vector<Component*> components;
	std::vector<GameObject*> children;

};