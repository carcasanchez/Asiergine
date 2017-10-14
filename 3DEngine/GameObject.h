#pragma once
#include <string>
#include <vector>

class Component;

enum ComponentType;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();

	Component* GetComponentByType(ComponentType type);
	
	const char* GetName() const { return name.c_str(); }

private:
	GameObject* parent = nullptr;
	std::string name;

	std::vector<Component*> components;
	std::vector<GameObject*> children;


};