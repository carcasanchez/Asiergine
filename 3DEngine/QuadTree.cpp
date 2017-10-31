#include "QuadTree.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include <vector>

//Node functions--------------------------------

QuadTreeNodeObj::~QuadTreeNodeObj()
{
}


void QuadTreeNodeObj::Insert(GameObject* game_object)
{
	game_objects.push_back(game_object);
}


void QuadTreeNodeObj::Remove()
{
	game_objects.clear();
}

void QuadTreeNodeObj::SetAABBToDraw()
{
	App->renderer3D->SetBoxToDraw(box);
	for (int i = 0; i < children.size(); ++i)
	{
		children[i]->SetAABBToDraw();
	}



}
void QuadTreeNodeObj::Partition()
{
	float3 center_point = { (box.minPoint.x + box.maxPoint.x) / 2,0,(box.minPoint.z + box.maxPoint.z) / 2 };
	float3 min_point1 = box.minPoint;
	float3 max_point1 = { center_point.x,box.maxPoint.y,center_point.z };
	float3 min_point2 = { center_point.x,box.minPoint.y,center_point.z };
	float3 max_point2 = box.maxPoint;
	float3 min_point3 = { center_point.x,box.minPoint.y,box.minPoint.z };
	float3 max_point3 = { box.maxPoint.x,box.maxPoint.y,center_point.z };
	float3 min_point4 = { box.minPoint.x,box.minPoint.y,center_point.z };
	float3 max_point4 = { center_point.x,box.maxPoint.y,box.maxPoint.z };
	QuadTreeNodeObj* child1 = new QuadTreeNodeObj(min_point1,max_point1);
	QuadTreeNodeObj* child2 = new QuadTreeNodeObj(min_point2, max_point2);
	QuadTreeNodeObj* child3 = new QuadTreeNodeObj(min_point3, max_point3);
	QuadTreeNodeObj* child4 = new QuadTreeNodeObj(min_point4, max_point4);
	if (child1->box.Size().x > minimum_aabb.Size().x)
	{
		children.push_back(child1);
		child1->Fill();
	}
	if (child2->box.Size().x > minimum_aabb.Size().x)
	{
		children.push_back(child2);
		child2->Fill();
	}
	if (child3->box.Size().x > minimum_aabb.Size().x)
	{
		children.push_back(child3);
		child3->Fill();
	}
	if (child4->box.Size().x > minimum_aabb.Size().x)
	{
		children.push_back(child4);
		child4->Fill();
	}
}

void QuadTreeNodeObj::Fill()
{
	std::vector<GameObject*> root_children = App->scene->root->GetChildrens();
	for (std::vector<GameObject*>::iterator it = root_children.begin(); it != root_children.end(); ++it)
	{
		if ((*it)->PutInQuadTree(this) == false)
		{
			if (this->box.Size().x > minimum_aabb.Size().x)
			{
				game_objects.clear();
				Partition();
				break;
			}
			else
				break;
		}
	}

}

void QuadTreeNodeObj::Clear()
{
	game_objects.clear();
	children.clear();
}

//QuadTree functions---------------------------------------
QuadTreeObj::QuadTreeObj()
{
	root.box.minPoint = float3(MIN_SCENE_POINT_X, MIN_SCENE_POINT_Y, MIN_SCENE_POINT_Z);
	root.box.maxPoint = float3(MAX_SCENE_POINT_X, MAX_SCENE_POINT_X, MAX_SCENE_POINT_X);
}
QuadTreeObj::~QuadTreeObj()
{}

void QuadTreeObj::Calcutale()
{
	root.Fill();
}

void QuadTreeObj::DeleteAll()
{
	root.Clear();
}



