#pragma once
#include "Component.h"
#include "GameObject.h"

class ComponentMaterial :public Component
{
public:
	ComponentMaterial(GameObject*, uint id);
	~ComponentMaterial();

	void OnEditor();

	uint texture_id = 0;
};

