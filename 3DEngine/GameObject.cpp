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
}

Component* GameObject::GetComponentByType(ComponentType type)
{
	if (components.empty() == false)
	{
		std::vector<Component*>::iterator compon = components.begin();
		while (compon != components.end())
		{
			if ((*compon)->GetType() == type)
			{
				return (*compon);
			}
			++compon;
		}
	}
	return nullptr;
}
