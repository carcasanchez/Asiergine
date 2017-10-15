#include "GameObject.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "Component.h"
#include "CompTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

GameObject::GameObject(const char* name): name(name)
{}

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

	for (int i = 0; i < children.size(); i++)
	{
	    //Check parent correlation
		if (children[i]->GetParent() != this)
		{
			LOG("YOU ARE ADOPTED");
			assert(1 == 0);
		}

		children[i]->Update();
	}
}

void GameObject::CleanUp()
{

	//Delete components
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		delete (*it);
	}

	//Delete children
	for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++)
	{
		(*it)->CleanUp();
		delete (*it);
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

void GameObject::SetParent(GameObject* new_parent)
{

	if (parent)
	{//Delete this object from the old parent childrens	
		for (std::vector<GameObject*>::iterator it = parent->children.begin(); it != parent->children.end(); it++)
		{
			if ((*it) == this)
			{
				parent->children.erase(it);
			}
		}
	}

	parent = new_parent;
	parent->children.push_back(this);

}

//CREATE COMPONENT METHODS----------------------------------------------------
CompTransform * GameObject::CreateComponent_Transform(float3 trans , float3 scale, Quat rot)
{
	if (GetComponentByType(COMPONENT_TRANSFORM) != nullptr)
	{
		LOG("ERROR: GameObject %s already has a transform component!", name.c_str());
		return nullptr;
	}

	CompTransform* new_transform = new CompTransform(this);

	new_transform->SetTranslation(trans.x, trans.y, trans.z);
	new_transform->SetScale(scale.x, scale.y, scale.z);
	new_transform->SetRotation(rot.x, rot.y, rot.z, rot.w);

	components.push_back(new_transform);

	LOG("Creating new Transform Component in %s", name.c_str());

	return new_transform;
}

ComponentMesh * GameObject::CreateComponent_Mesh(float * ver, uint * ind, uint num_vert, uint num_ind, uint texture_id, float * texture_coords)
{
	ComponentMesh* new_mesh = new ComponentMesh(this, ver, ind, num_vert, num_ind, texture_id, texture_coords);
	
	components.push_back(new_mesh);
	LOG("Creating new Mesh with %i vertices in %s", new_mesh->GetNumVertices(), name.c_str());
	return new_mesh;
}

ComponentMaterial * GameObject::CreateComponent_Material()
{
	ComponentMaterial* new_mat = new ComponentMaterial(this);

	components.push_back(new_mat);
	LOG("Creating new Material  in %s", name.c_str());
	return new_mat;
}
