#pragma once
#include "Component.h"
#include "Globals.h"
#include "Light.h"


class ComponentLight : public Component
{
public:
	ComponentLight(GameObject* game_object, Light l);
	~ComponentLight();

	std::string name;
		
	void Update(float real_dt, float game_dt);
	void OnEditor();

	
	bool IsActive() const { return active; }

	//Scene serialization------------------------
	uint PrepareToSave() const;
	void Save(char* &cursor) const;

private:
	Light light;

};