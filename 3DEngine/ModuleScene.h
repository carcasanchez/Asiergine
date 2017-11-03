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

	GameObject* CreateGameObject(const char* name, GameObject* parent = nullptr, uint UID = 0);


	

	GameObject* root = nullptr;

	bool debug_normals = false;
	bool debug_boxes = false;

	QuadTreeObj scene_quadtree;

	bool wants_to_reset = false;
	bool wants_to_save = false;
	bool wants_to_load = false;

	//Cleans Scene
	void CleanScene();

	//Cleans and adds a camera to scene
	void ResetScene();

	


	std::string scene_name;

	

private:
	bool SaveConfig(JSON_Object* config_data);

};

#endif 