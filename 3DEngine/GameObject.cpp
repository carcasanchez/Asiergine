#include "GameObject.h"
#include "Component.h"
#include "Application.h"
#include "Globals.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	std::vector<Component*>::iterator it = components.begin();
	while (it != components.end())
	{
		(*it)->Update();
		++it;
	}
}

Component* GameObject::GetComponentByType(ComponentType type)
{
	if (components.empty() == false)
	{
		std::vector<Component*>::iterator it = components.begin();
		while (it != components.end())
		{
			if ((*it)->GetType() == type)
			{
				return (*it);
			}
			++it;
		}
	}
	return nullptr;
}
