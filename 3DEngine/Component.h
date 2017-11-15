#pragma once
#include "Globals.h"

class GameObject;

enum ComponentType
{
	COMPONENT_TRANSFORM,
	COMPONENT_MESH,
	COMPONENT_MATERIAL,
	COMPONENT_CAMERA,
	COMPONENT_LIGHT
};

class Component
{
public:
	Component(GameObject* game_object);
	virtual ~Component();

	virtual void Enable();
	virtual void Update(float real_dt, float game_dt);
	virtual void Disable();

	virtual bool IsActive() { return active; }
	void SetActive(bool val) { active = val; }

	const char* GetComponentNameByType();

	ComponentType GetType() const { return type; }
	GameObject* GetGameObject() const { return game_object; }
	uint GetID()
	{
		return UID;
	}
	void SetID(uint newID)
	{
		UID = newID;
	}

	virtual void OnEditor() {}

	//Scene serialization----------------------------------- 
	virtual uint PrepareToSave() const { return 0; };
	virtual void Save (char* &cursor) const {};
	//----------------------------------------------------

	bool active = true;
	bool deleted = false;
protected:
	GameObject* game_object = nullptr;
	ComponentType type;

	uint UID = 0;
};
