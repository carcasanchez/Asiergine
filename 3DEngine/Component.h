#pragma once

class GameObject;
enum ComponentType
{
	COMPONENT_TRANSFORM,
	COMPONENT_MESH,
	COMPONENT_MATERIAL
};

class Component
{
public:
	Component(GameObject* game_object);
	~Component();

	virtual void Enable();
	virtual void Update();
	virtual void Disable();

	bool IsActive() { return active; }
	void SetActive(bool val) { active = val; }

	ComponentType GetType() const { return type; }
	GameObject* GetGameObject() const { return game_object; }

	virtual void OnEditor() {}


protected:
	bool active = true;
	GameObject* game_object = nullptr;
	ComponentType type;
};
