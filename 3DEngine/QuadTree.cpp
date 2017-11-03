#include "Application.h"
#include "QuadTree.h"
#include ".\mmgr\mmgr.h"
#include "ModuleRenderer3D.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include <vector>

//Node functions--------------------------------

QuadTreeNodeObj::~QuadTreeNodeObj()
{
	Clear();
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
	QuadTreeNodeObj* child1 = new QuadTreeNodeObj(min_point1, max_point1, max_game_objects);
	QuadTreeNodeObj* child2 = new QuadTreeNodeObj(min_point2, max_point2, max_game_objects);
	QuadTreeNodeObj* child3 = new QuadTreeNodeObj(min_point3, max_point3, max_game_objects);
	QuadTreeNodeObj* child4 = new QuadTreeNodeObj(min_point4, max_point4, max_game_objects);

	children.push_back(child1);
	child1->Fill();
	children.push_back(child2);
	child2->Fill();
	children.push_back(child3);
	child3->Fill();
	children.push_back(child4);
	child4->Fill();
}

void QuadTreeNodeObj::Fill()
{
	if (this->box.Size().x <= minimum_aabb.Size().x)
		min_size_posible = true;

	std::vector<GameObject*> root_children = App->scene->root->GetChildrens();
	for (std::vector<GameObject*>::iterator it = root_children.begin(); it != root_children.end(); ++it)
	{
		if ((*it)->PutInQuadTree(this) == false)
		{
			if (!min_size_posible)
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
	for (std::vector<QuadTreeNodeObj*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		RELEASE(*it);
	}
	children.clear();
	game_objects.clear();
}

//QuadTree functions---------------------------------------
QuadTreeObj::QuadTreeObj()
{
	root.box.minPoint = min_scene_point;
	root.box.maxPoint = max_scene_point;
	
}
QuadTreeObj::~QuadTreeObj()
{
	root.Clear();
}

void QuadTreeObj::Calculate()
{
	root.Clear();
	root.SetMaxNumObj(max_game_objects);
	root.Fill();
}



void QuadTreeObj::ResizeRoot(float3 min, float3 max)
{
	root.Resize(min, max);
}



