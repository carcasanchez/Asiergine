#pragma once
#include "Component.h"
#include "GameObject.h"

class ComponentMaterial :public Component
{
public:
	ComponentMaterial(GameObject*, uint id);
	~ComponentMaterial();

	void OnEditor();

	bool IsActive() { return active; }

	uint texture_id = 0;
	std::string texture_name;
};

