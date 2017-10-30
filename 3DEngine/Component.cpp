#include "Component.h"
#include "Application.h"
#include "GameObject.h"


Component::Component(GameObject* game_object) : game_object(game_object)
{
	LCG rand_gen;
	UID = rand_gen.Int(1, 32000);
}

Component::~Component()
{
}

void Component::Enable()
{
	active = true;
}

void Component::Update()
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

	default:
		break;
	}
}