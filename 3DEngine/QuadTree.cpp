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
		App->renderer3D->SetBoxToDraw(children[i]->box);
	}
}
void QuadTreeNodeObj::Partition()
{
	float distance = (box.CornerPoint(0).x - box.CornerPoint(1).x) / 2;
	float3 min_point1 = box.minPoint;
	float3 max_point1 = { box.maxPoint.x - distance, box.maxPoint.y, box.maxPoint.z - distance };
	QuadTreeNodeObj* child1 = new QuadTreeNodeObj(min_point1,max_point1);
	QuadTreeNodeObj* child2 = new QuadTreeNodeObj();
	QuadTreeNodeObj* child3 = new QuadTreeNodeObj();
	QuadTreeNodeObj* child4 = new QuadTreeNodeObj();
		
		children.push_back(child1);
		children.push_back(child2);
		children.push_back(child3);
		children.push_back(child4);
		for (int i = 0; i < children.size(); ++i)
		{
			children[i]->Fill();			
		}

}

void QuadTreeNodeObj::Fill()
{
	if (App->scene->root->PutInQuadTree(this) == false)
	{
		game_objects.clear();
		Partition();
	}

}

void QuadTreeNodeObj::Clear()
{
	/*std::vector<QuadTreeNodeObj*>::iterator it = children.end();
	while (it != children.begin())
	{
		children.pop_back((*it));
		--it;
	}
	std::vector<GameObject*>::iterator it1 = game_objects.end();
	while (it1 != game_objects.begin())
	{
		game_objects.pop_back((*it));
		--it;
	}*/
}


/*bool QuadTreeNodeObj::Intersect(std::vector<GameObject*>& game_bjects, const & primitive)
{
	return false;
}*/


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

/*void QuadTreeObj::Draw()
{
	int i = 0;
	root->Create(float3::zero, { 12,12,12 });
	std::vector<GameObject*>::iterator it = game_objects.begin();
	while (it != game_objects.end())
	{
		++i;
		++it;
	}
	if (i > root->GetMaxGameObjects())
	{
		root->Create({ 1.5,6,1.5 }, { 3,12,3 });
		root->Create({ -1.5,6,-1.5 }, { 3,12,3 });
		root->Create({ 1.5,6,-1.5 }, { 3,12,3 });
		root->Create({ -1.5,6,1.5 }, { 3,12,3 });
	}

	
}*/



