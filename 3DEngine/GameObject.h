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
	void CleanUp();

	Component* GetComponentByType(ComponentType type);
	
	const char* GetName() const { return name.c_str(); }

	const GameObject* GetParent() { return parent; }
	std::vector<GameObject*> GetChildrens() { return children;}
	void SetParent(GameObject*);

private:

	GameObject* parent = nullptr;
	std::string name;

	std::vector<Component*> components;
	std::vector<GameObject*> children;

};