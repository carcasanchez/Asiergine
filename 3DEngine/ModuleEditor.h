#pragma once
#include "Module.h"
#include "imgui\imgui.h"
#include "MathGeoLib\include\Math.h"


class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);

	// Destructor
	~ModuleEditor();
	
	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void ManageMainMenuBar();

	void ManageExampleWindow();

	void ShowExampleWindow_option();
	void ExitEditor_option();

	void ManageConsole();

private:

	bool test_window_open = false;
	bool quit_editor = false;
	bool console_open = false;


	math::Ray ray;
	math::Sphere sphere;
	math::Capsule caps;


};


