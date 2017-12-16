#pragma once
#include "Component.h"
class ComponentReverb :	public Component
{
public:
	ComponentReverb(GameObject*);
	~ComponentReverb();

	void Update(float real_dt, float game_dt);
	void OnEditor();
	bool CheckCollision(float3 p) const;

	std::string target_bus = "";
	float value = 0.0;

	uint PrepareToSave() const;
	void Save(char *& cursor) const;

private:
	math::OBB box;
	
};

