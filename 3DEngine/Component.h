#pragma once

enum ComponentType
{
	COMPONENT_TRANSFORM,
	COMPONENT_MESH,
	COMPONENT_MATERIAL
};

class Component
{
public:
	Component(ComponentType type);
	~Component();

	virtual void Enable();
	virtual void Update();
	virtual void Disable();
private:
	bool active = true;

	ComponentType type;

};
