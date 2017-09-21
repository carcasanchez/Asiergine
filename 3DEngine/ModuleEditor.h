#ifndef _MODULE_EDITOR_H_
#define _MODULE_EDITOR_H_
#include "Module.h"
#include "Console.h"
#include "imgui\imgui.h"
#include "MathGeoLib\include\Math.h"


class ModuleEditor : public Module
{
public:
	ModuleEditor(bool start_enabled = true);

	// Destructor
	~ModuleEditor();
	
	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void DrawInConsole(const char*);


private:
		
	AppConsole console;
	
	bool test_window_open = false;
	bool quit_editor = false;
	bool console_open = false;
	bool about_engine_open = false;
	bool configuration_open = true;
	bool application_open = true;

	void ManageMainMenuBar();
	void ManageExampleWindow();
	void ManageAboutWindow();
	void ManageConsole();
	void ManageConfigurationWindow();

	void ShowExampleWindow_option();
	void Window_option();
	void ExitEditor_option();


};

#endif

