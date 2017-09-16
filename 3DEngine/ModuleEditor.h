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
	bool Update();
	bool CleanUp();

private:
	ImGuiID demo_window;
};


