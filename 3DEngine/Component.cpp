#include "Component.h"
#include "Application.h"
#include "GameObject.h"


Component::Component(GameObject* game_object) : game_object(game_object)
{
	LCG rand_gen;
	UID = rand_gen.Int();
}

Component::~Component()
{
}

void Component::Enable()
{
	active = true;
}

void Component::Update(float real_dt, float game_dt)
{
}

void Component::Disable()
{
	active = false;
}

const char* Component::GetComponentNameByType()
{
	switch (type)
	{
	case COMPONENT_TRANSFORM:
		return "Component Transform";
		
	case COMPONENT_MESH:
		return "Component Mesh";
		
	case COMPONENT_MATERIAL:
		return "Component Material";
		
	case COMPONENT_CAMERA:
		return "Component Camera";

	case COMPONENT_LIGHT:
		return "Component Light";

	case COMPONENT_AUDIO:
		return "Component Audio";

	case COMPONENT_MOVEMENT:
		return "Component Movement";

	case COMPONENT_REVERB:
		return "Component Reverb";

	default:
		break;
	}
}