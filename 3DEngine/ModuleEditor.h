#pragma once
#include "Module.h"
#include "Console.h"
#include "imgui\imgui.h"
#include "MathGeoLib\include\Math.h"
/*#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>*/

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
	void DrawInConsole(const char*);


	void GetInputEvent(SDL_Event* e);

	bool calculate_fps_graph = false;

	
private:

	AppConsole console;

	bool test_window_open = false;
	bool quit_editor = false;
	bool console_open = false;
	bool about_engine_open = false;
	bool hardware_open = false;
	bool configuration_open = false;
	bool hierarchy_open = false;
	bool inspector_open = false;

	bool unlimitedFramerate = true;



	//--indice elements
float vertex2[24] =
	{
		20.0, 20.0,  20.0,
		20.0, 20.0,  10.0,
		10.0, 20.0,  20.0,
		10.0, 20.0,  10.0,

		20.0, 10.0, 20.0,
		20.0, 10.0, 10.0,
		10.0,  10.0, 20.0,
		10.0, 10.0, 10.0,
	};

	int indices[36] =		
		{1, 0, 2,  3, 1, 2,	
		3, 2, 6,  7, 3, 6,	
		3, 7, 5,  1, 3, 5,	
		7, 6, 4,  5, 7, 4,		
		5, 4, 0,  1, 5, 0,		
		0, 4, 6,  2, 0, 6,
};


	uint my_indices = 0;

	


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
	void ManageHierarchyWindow();
	void ManageInspectorWindow();


	void ShowExampleWindow_option();
	void Window_option();
	void ExitEditor_option();
	

	uint array_id2 = 0;

	Plane_prim plane;
};

