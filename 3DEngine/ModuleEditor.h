#pragma once
#include "Module.h"
#include "Console.h"
#include "imgui\imgui.h"
#include "MathGeoLib\include\Math.h"

#include "Imgui/imgui_impl_sdl_gl3.h"


#define MAX_FPS_LOG 100

typedef union SDL_Event;
typedef unsigned char GLubyte;
typedef float GLfloat;
class GameObject;

class ModuleEditor : public Module
{
public:
	ModuleEditor(bool start_enabled = true);

	// Destructor
	~ModuleEditor();

	bool Init(const JSON_Object* config_data);
	bool Start();
	update_status PreUpdate(float real_dt, float game_dt);
	update_status Update(float real_dt, float game_dt);
	update_status PostUpdate(float real_dt, float game_dt);
	bool CleanUp();

	void DrawUI();
	void DrawInConsole(const char*);


	void GetInputEvent(SDL_Event* e);
	bool SaveConfig(JSON_Object* config_data);
	bool calculate_fps_graph = false;

	void SelectObject(GameObject*);

	GameObject* GetSelectedGameObject() const { return selected_object; }

	void UnselectAll()
	{
		objects_picked.clear();
		selected_object = nullptr;
	}

	bool IsInputLocked() const
	{
		return input_locked;
	}
	bool IsSelectionLocked() const
	{
		return selection_locked;
	}

	bool BakeQuadtreeOpen() const
	{
		return bake_menu_open;
	}

	void LockInput()
	{
		input_locked = true;
	}

	void LockSelection()
	{
		selection_locked = true;
	}

	void UnLockSelection()
	{
		selection_locked = false;
	}
	
	void LockGizmos()
	{
		gizmos_locked = true;
	}

	void UnLockGizmos()
	{
		gizmos_locked = false;
	}

	bool AreGizmosLocked() const
	{
		return gizmos_locked;
	}

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
	bool hierarchy_open = false;
	bool play_window_open = true;

	bool unlimitedFramerate = true;
	bool bake_quadtree = false;

	bool save_window_open = false;
	bool load_window_open = false;

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


	void ManagePlayAppOptions();

	void ManageMainMenuBar();
	void ManageExampleWindow();
	void ManageAboutWindow();
	void ManageConsole();
	void ManageConfigurationWindow();
	void ManageHierarchyWindow();
	void ManageHierarchyChildren(GameObject*);
	void ManageInspectorWindow();
	void ManageSaveWindow();

	//Configuration Window
	void ConfigAppMenu();
	void ConfigWindowMenu();
	void ConfigCameraMenu();
	void ConfigHardSpecsMenu();
	void ConfigRenderingMenu();
	void ConfigAudioMenu();
	void ConfigInputMenu();
	void ConfigBakeMenu();
	void ConfigVramUsageMenu();
	void ConfigTimeMenu();

	//Main menu bar options
	void ShowExampleWindow_option();
	void File_option();
	void Window_option();

	bool bake_menu_open = false;
	bool input_locked = false;
	bool selection_locked = false;
	bool gizmos_locked = false;
	bool selected = false;

	GameObject* selected_object = nullptr;
	std::vector<GameObject*> objects_picked;

	//Editor sizes
	void Resize();
	math::float2 config_size;
	math::float2 config_pos;
	math::float2 hierarchy_size;
	math::float2 hierarchy_pos;
	math::float2 inspector_size;
	math::float2 inspector_pos;


};

