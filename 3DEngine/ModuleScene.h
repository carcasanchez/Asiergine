#ifndef __ModuleScene_H__
#define __ModuleScene_H__

#include "Globals.h"
#include "Module.h"


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

private:
	bool SaveConfig(JSON_Object* config_data);


	bool SaveScene();

};

#endif 