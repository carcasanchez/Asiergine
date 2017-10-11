#pragma once
#include "Module.h"
#include "Console.h"
#include "imgui\imgui.h"
#include "MathGeoLib\include\Math.h"

#include "Imgui/imgui_impl_sdl_gl3.h"
#include "Primitive.h"

#define MAX_FPS_LOG 100

typedef union SDL_Event;
typedef unsigned char GLubyte;
typedef float GLfloat;


class ModuleEditor : public Module
{
public:
	ModuleEditor(bool start_enabled = true);

	// Destructor
	~ModuleEditor();

	bool Init(const JSON_Object* config_data);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void DrawUI();
	void DrawGeometry();
	void DrawInConsole(const char*);


	void GetInputEvent(SDL_Event* e);
	bool SaveConfig(JSON_Object* config_data);
	bool calculate_fps_graph = false;

	
private:

	AppConsole console;

	bool test_window_open = false;
	bool quit_editor = false;
	bool console_open = false;
	bool about_engine_open = false;
	bool hardware_open = false;
	bool configuration_open = false;
	bool inspector_open = false;
	bool camera_config_open = false;

	bool unlimitedFramerate = true;




//	UINT nDevices;
//	PRAWINPUTDEVICELIST pRawInputDeviceList;

	//Framerate calculation
	std::vector<float> fps_log;
	std::vector<float> ms_log;

	//Vram calculation
	std::vector<float> vram_log;
	std::vector<float> curr_vram_log;

	bool fullscreen = false;
	bool resizable = true;
	bool borderless = false;
	bool fulldesktop = false;
	int max_fps = 0;


	void ManageMainMenuBar();
	void ManageExampleWindow();
	void ManageAboutWindow();
	void ManageConsole();
	void ManageConfigurationWindow();
	void ManageInspectorWindow();


	void ShowExampleWindow_option();
	void Window_option();
	void ExitEditor_option();
	
	Plane_prim plane;

	bool debug_draw = false;

};

