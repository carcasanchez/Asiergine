#include "Component.h"
#include "Application.h"
#include "GameObject.h"
#include "Globals.h"

Component::Component(ComponentType type, GameObject* game_object) : type(type), game_object(game_object)
{
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
