#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Globals.h"
#include "Module.h"
#include "QuadTree.h"

class GameObject;

class ModuleScene : public Module
{
public:

	ModuleScene(bool start_enabled = true);

	// Destructor
	virtual ~ModuleScene();

	bool Init(const JSON_Object* config_data);
	update_status Update(float dt);
	bool CleanUp();

	GameObject* CreateGameObject(const char* name, GameObject* parent = nullptr);

	GameObject* root = nullptr;

	bool debug_normals = false;
	bool debug_boxes = false;

	QuadTreeObj scene_quadtree;

	GameObject* camera;
	GameObject* box;
	GameObject* box1;
	GameObject* box2;
	GameObject* box3;
	GameObject* box4;
	GameObject* box5;
	GameObject* box6;
	GameObject* box7;
	GameObject* box8;
	GameObject* box9;
	GameObject* box10;
	GameObject* box11;
	GameObject* box12;
	GameObject* box13;
	GameObject* box14;
	GameObject* box15;

private:
	bool SaveConfig(JSON_Object* config_data);


	bool SaveScene();

};

#endif 