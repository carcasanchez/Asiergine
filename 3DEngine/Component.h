#pragma once

class GameObject;
enum ComponentType
{
	COMPONENT_TRANSFORM,
	COMPONENT_MESH,
	COMPONENT_MATERIAL,
	COMPONENT_CAMERA
};

class Component
{
public:
	Component(GameObject* game_object);
	virtual ~Component();

	virtual void Enable();
	virtual void Update();
	virtual void Disable();

	virtual bool IsActive() { return active; }
	void SetActive(bool val) { active = val; }

	const char* GetComponentNameByType();

	ComponentType GetType() const { return type; }
	GameObject* GetGameObject() const { return game_object; }

	virtual void OnEditor() {}

	bool active = true;

protected:
	GameObject* game_object = nullptr;
	ComponentType type;
};
