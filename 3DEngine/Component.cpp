#include "Component.h"
#include "Application.h"
#include "GameObject.h"
#include "Globals.h"

Component::Component(GameObject* game_object) : game_object(game_object)
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
