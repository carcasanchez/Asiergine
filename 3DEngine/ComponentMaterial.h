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

	uint GetTextureID() const
	{
		return texture_id;
	}

	std::string texture_name;

private:
	uint texture_id = 0;
	bool change_text_window = false;
};

