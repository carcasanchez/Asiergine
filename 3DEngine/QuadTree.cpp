#include "QuadTree.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
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
	game_objects.push_back(App->scene->root);
	if (game_objects.size() > max_game_objects)
	{
		/*while(game_objects.size() > 0)
			game_objects.pop_back();*/
		QuadTreeNodeObj child1, child2, child3, child4;
		child1.box.minPoint = float3(-20, -20, 0);
		child1.box.minPoint = float3(0, 20, 0);
		children.push_back(&child1);
		children.push_back(&child2);
		children.push_back(&child3);
		children.push_back(&child4);
		for (int i = 0; i < children.size(); ++i)
		{
			children[i]->SetAABBToDraw();
			children[i]->Partition();
		}
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
	root.box.minPoint = float3(-20, -20, -20);
	root.box.maxPoint = float3(20, 20, 20);
}
QuadTreeObj::~QuadTreeObj()
{}

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



