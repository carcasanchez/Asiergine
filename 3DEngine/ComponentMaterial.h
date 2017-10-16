#pragma once
#include "Component.h"
#include "GameObject.h"

class ComponentMaterial :public Component
{
public:
	ComponentMaterial(GameObject*, uint id);
	~ComponentMaterial();

	uint texture_id = 0;
};

