#pragma once
#include "Component.h"
class ComponentReverb :	public Component
{
public:
	ComponentReverb(GameObject*);
	~ComponentReverb();

	void Update(float real_dt, float game_dt);

private:
	math::OBB box;
};

