#ifndef _MODULE_EDITOR_H_
#define _MODULE_EDITOR_H_
#include "Module.h"
#include "Console.h"
#include "imgui\imgui.h"
#include "MathGeoLib\include\Math.h"


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

	//Geometry
	float vertex[120];

	//--indice elements
	GLfloat vertex2[24] =
	{
		2.0, -1.0,  1.0,
		4.0, -1.0,  1.0,
		4.0,  1.0,  1.0,
		2.0,  1.0,  1.0,

		2.0, -1.0, -1.0,
		4.0, -1.0, -1.0,
		4.0,  1.0, -1.0,
		2.0,  1.0, -1.0,
	};

	GLubyte indices[36] =		
		{0, 1, 2,
		2, 3, 0,	
		1, 5, 6,
		6, 2, 1,	
		7, 6, 5,
		5, 4, 7,	
		4, 0, 3,
		3, 7, 4,		
		4, 5, 1,
		1, 0, 4,		
		3, 2, 6,
		6, 7, 3,
};

	int num_of_vertex= 0;
	uint my_indices = 0;
	int num_indices = 36;

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



	uint array_id = 0;
	uint array_id2 = 0;
};

#endif

