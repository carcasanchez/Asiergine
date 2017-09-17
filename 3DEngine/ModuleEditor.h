#pragma once
#include "Module.h"
#include "imgui\imgui.h"

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

	void ShowExampleAppMainMenuBar();
	void ShowExampleWindow();
	void ShowQuitWindow();
	void ExitEditor();

private:
	bool test_window_open = false;

	bool quit_editor = false;
};


