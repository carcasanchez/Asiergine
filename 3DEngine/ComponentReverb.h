#pragma once
#include "Component.h"
class ComponentReverb :	public Component
{
public:
	ComponentReverb(GameObject*);
	~ComponentReverb();

	void Update(float real_dt, float game_dt);
	void OnEditor();


	std::string target_bus = "";
	float value = 0.0;

private:
	math::OBB box;
	
};

