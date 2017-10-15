#include "ComponentMaterial.h"


ComponentMaterial::ComponentMaterial(GameObject* g):Component(g)
{
	type = COMPONENT_MATERIAL;
}


ComponentMaterial::~ComponentMaterial()
{
}
