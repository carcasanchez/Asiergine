#include "Globals.h"
#include "Application.h"
#include <filesystem>
#include <experimental\filesystem>

#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include ".\mmgr\mmgr.h"
#include "ModuleEditor.h"
#include "GameObject.h"
#include "imgui\ImGuizmo.h"



ModuleEditor::ModuleEditor( bool start_enabled) : Module( start_enabled)
{
	name = "editor";
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init(const JSON_Object* config_data)
{
	bool ret = true;

	ImGui_ImplSdlGL3_Init(App->window->window);
	
		//LoadData from Config
	assert(config_data != nullptr);

	test_window_open = json_object_dotget_boolean(config_data, "test_window_open");
	console_open = json_object_dotget_boolean(config_data, "console_open");
	about_engine_open = json_object_dotget_boolean(config_data, "about_engine_open");
	hardware_open = json_object_dotget_boolean(config_data, "hardware_open");
	configuration_open = json_object_dotget_boolean(config_data, "configuration_open");
	inspector_open = json_object_dotget_boolean(config_data, "inspector_open");
	camera_config_open = json_object_dotget_boolean(config_data, "camera_config_open");
	hierarchy_open = json_object_dotget_boolean(config_data, "hierarchy_open");

	return ret;
}

bool ModuleEditor::Start()
{
	bool ret = true;
	ImGui_ImplSdlGL3_NewFrame(App->window->window);

	//Imgui style change
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 0.0f;
	style.WindowRounding = 0.0f;
	style.ScrollbarRounding = 0.0f;
	style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.82f, 0.55f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.10f, 0.43f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.03f, 0.06f, 0.26f, 0.31f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.17f, 0.14f, 0.52f, 0.90f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.50f, 0.83f, 0.43f, 0.43f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.99f, 0.99f, 0.99f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.49f, 0.22f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.36f, 0.06f, 0.06f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.35f, 0.09f, 0.09f, 0.45f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.51f, 0.51f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 1.00f, 0.87f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.40f, 0.55f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.36f, 0.49f, 0.60f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.95f, 0.18f, 0.18f, 0.30f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.90f, 0.56f, 0.40f, 0.40f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.40f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.42f, 0.66f, 0.33f, 0.50f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.72f, 0.53f, 0.71f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.31f, 0.83f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.80f, 0.14f, 0.14f, 0.72f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.64f, 0.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.73f, 0.06f, 0.06f, 0.94f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.97f, 0.41f, 0.41f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.09f, 0.09f, 0.60f, 0.80f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.60f, 0.06f, 0.06f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.88f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.93f, 0.02f, 0.02f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.09f, 0.93f, 0.92f, 0.36f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.12f, 0.20f, 0.98f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.04f, 0.90f, 0.90f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(1.00f, 0.00f, 0.00f, 0.61f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.19f, 0.19f, 0.47f, 0.60f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.97f, 0.33f, 0.33f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.08f, 0.73f, 0.39f, 0.79f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.08f, 0.89f, 0.04f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.96f, 0.92f, 0.87f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.66f, 0.56f, 0.56f, 0.35f);

	

	//reserve for fps_log
	fps_log = std::vector<float>(MAX_FPS_LOG, 0);
	ms_log = std::vector<float>(MAX_FPS_LOG, 0);
	
	return ret;
}



update_status ModuleEditor::PreUpdate(float real_dt, float game_dt)
{
	update_status ret = UPDATE_CONTINUE;
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	ImGuizmo::BeginFrame();
	return ret;
}

update_status ModuleEditor::Update(float real_dt, float game_dt)
{	

	update_status ret = UPDATE_CONTINUE;
	
	//Check if Asset has been dropped and load geometry
	const char* f_path = App->input->GetFileDropped();	
	if (f_path != nullptr)
	{
		App->resource_m->CheckFileStatus(f_path);
	}

			
	return ret;
}

update_status ModuleEditor::PostUpdate(float real_dt, float game_dt)
{
	update_status ret = UPDATE_CONTINUE;	

	
	if (quit_editor)
		ret = UPDATE_STOP;
	return ret;
}

bool ModuleEditor::CleanUp()
{
	bool ret = true;

	//Shuts down imGui
	ImGui_ImplSdlGL3_Shutdown();

	return ret;
}



//Draw functions------------------------------------------------------------------------
void ModuleEditor::DrawUI()
{
	input_locked = ImGui::IsMouseHoveringAnyWindow();

	ManagePlayAppOptions();
	ManageSaveWindow();
	ManageMainMenuBar();
	ManageAboutWindow();
	ManageConsole();
	ManageConfigurationWindow();
	ManageExampleWindow();
	ManageHierarchyWindow();
	ManageInspectorWindow();

	ImGui::Render();
}



void ModuleEditor::ManagePlayAppOptions()
{
	ImGui::SetNextWindowSize(ImVec2(300, 80));
	ImGui::SetNextWindowPos(ImVec2(400, 20));

	ImGui::Begin("Play Game");

	if (App->IsAppRunning())
	{
	
		if (ImGui::Button("Stop"))
		{
			App->StopApp();
			App->scene->scene_quadtree.Calculate();
		}

		ImGui::SameLine();
		if (!App->IsAppPaused())
		{
			static float speed = App->GetGameSpeed();
			
			if (ImGui::Button("Pause"))
			{
				App->PauseApp();
			}
			
			if (ImGui::DragFloat("Game Speed", &speed, 0.01, 0.0, 1.0))
			{
				App->SetGameSpeed(speed);
			}			
				
		}	

		if (App->IsAppPaused())
		{
			if(ImGui::Button("Restart"))
				App->UnPauseApp();
			ImGui::SameLine();
			if (ImGui::Button("Update Once"))
				App->UpdateOnce();
		}
	}
	else
	{
		if (ImGui::Button("Play"))
		{
			App->PlayApp();
		}
	}


	ImGui::End();
}

//MAIN MENU BAR management------------------------------------------------------------------------
void ModuleEditor::ManageMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{

		if (ImGui::BeginMenu("File"))
		{			
			File_option();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			Window_option();
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("About"))
		{
			about_engine_open = true;
		}
				
	
		ImGui::EndMainMenuBar();
	}
}

void ModuleEditor::ShowExampleWindow_option()
{
	if (ImGui::MenuItem("Show"))
	{
		test_window_open = true;
	}

	if (ImGui::MenuItem("Hide"))
	{
		test_window_open = false;
	}
}

void ModuleEditor::Window_option()
{
	if (ImGui::MenuItem("Console"))
		console_open = true;

	if (ImGui::MenuItem("Configuration"))
		configuration_open = true;

	if (ImGui::MenuItem("Hierarchy"))
		hierarchy_open = true;

	if (ImGui::MenuItem("Inspector"))
		inspector_open = true;	
}

void ModuleEditor::File_option() 
{

	if(!App->IsAppRunning() && !App->IsAppPaused())
	{ 
		if (ImGui::MenuItem("New scene"))
			App->scene->wants_to_reset = true;

		if (ImGui::MenuItem("Save scene"))
			save_window_open = true;

		if (ImGui::MenuItem("Load scene"))
			load_window_open = true;
	}

	if (ImGui::MenuItem("Exit"))
		quit_editor = true;
}


//EMERGENT WINDOWS management

//Example window management------------------------------------------------------------------------
void ModuleEditor::ManageExampleWindow()
{
	if (test_window_open == true)
		ImGui::ShowTestWindow();
}

//About window------------------------------------------------------------------------
void ModuleEditor::ManageAboutWindow()
{
	if (about_engine_open)
	{
		ImGui::Begin("About", &about_engine_open, ImGuiWindowFlags_ShowBorders);
		ImGui::TextWrapped("ASIERGINE");
		ImGui::Separator();
		ImGui::TextWrapped("Made by");
		if(ImGui::Button("Carlos Cabreira Sanchez"))
			ShellExecute(NULL, "open", "https://github.com/carcasanchez", NULL, NULL, SW_SHOWNORMAL);
		ImGui::TextWrapped("and");
		if (ImGui::Button("Asier Iglesias Gavarro"))
			ShellExecute(NULL, "open", "https://es.linkedin.com/in/asier-iglesias-gavarro", NULL, NULL, SW_SHOWNORMAL);
	
		ImGui::Separator();
		ImGui::TextWrapped("3D Game Engine created for educational purposes (Work in Progress)");
		ImGui::Separator(); 
		ImGui::Separator();

		ImGui::TextWrapped("LIBRARIES UNDER USAGE:  ---All credits to the authors");
		if (ImGui::Button("Bullet 2.x"))
			ShellExecute(NULL, "open", "http://bulletphysics.org/wordpress/", NULL, NULL, SW_SHOWNORMAL);
		if (ImGui::Button("Glew 7.0"))
			ShellExecute(NULL, "open", "http://glew.sourceforge.net/", NULL, NULL, SW_SHOWNORMAL);
		if (ImGui::Button("MathGeoLib"))
			ShellExecute(NULL, "open", "http://clb.demon.fi/MathGeoLib/nightly/", NULL, NULL, SW_SHOWNORMAL);
		if (ImGui::Button("ImGui"))
			ShellExecute(NULL, "open", "https://github.com/ocornut/imgui", NULL, NULL, SW_SHOWNORMAL);
		if (ImGui::Button("SDL 2"))
			ShellExecute(NULL, "open", "https://www.libsdl.org/download-2.0.php", NULL, NULL, SW_SHOWNORMAL);
		if (ImGui::Button("OpenGL 3"))
			ShellExecute(NULL, "open", "https://www.khronos.org/registry/OpenGL-Refpages/es3.0/", NULL, NULL, SW_SHOWNORMAL);
		if (ImGui::Button("MMGR by Fluid Studios"))
			ShellExecute(NULL, "open", "http://www.paulnettle.com/", NULL, NULL, SW_SHOWNORMAL);
		if (ImGui::Button("Parson"))
			ShellExecute(NULL, "open", "https://github.com/kgabis/parson", NULL, NULL, SW_SHOWNORMAL);
		if (ImGui::Button("Devil"))
			ShellExecute(NULL, "open", "http://openil.sourceforge.net/", NULL, NULL, SW_SHOWNORMAL);
		if (ImGui::Button("Assimp"))
			ShellExecute(NULL, "open", "http://assimp.sourceforge.net/", NULL, NULL, SW_SHOWNORMAL);



		ImGui::Separator();
		ImGui::Separator();

		ImGui::TextWrapped("This software is under the MIT licence");
		if (ImGui::Button("Click here for more information about the MIT license"))
			ShellExecute(NULL, "open", "https://github.com/carcasanchez/Asiergine/blob/master/LICENSE", NULL, NULL, SW_SHOWNORMAL);
	
		ImGui::Separator();
		if (ImGui::Button("Click here to check the code repository"))
			ShellExecute(NULL, "open", "https://github.com/carcasanchez/Asiergine/", NULL, NULL, SW_SHOWNORMAL);

		ImGui::End();
	}
}


//Console management------------------------------------------------------------------------
void ModuleEditor::ManageConsole()
{
	if(console_open)
		console.Draw("Asiergine Console", &console_open);
}

void ModuleEditor::DrawInConsole(const char * to_console)
{
		console.AddLog(to_console);
}


//Configuration window------------------------------------------------------------------------
void ModuleEditor::ManageConfigurationWindow()
{
	if (configuration_open)
	{
		ImGui::Begin("Configuration", &configuration_open, ImGuiWindowFlags_ShowBorders);
		
		//Application submenu
		if (ImGui::CollapsingHeader("Application"))
		{
			ConfigAppMenu();
		}

		//Time submenu
		if (ImGui::CollapsingHeader("Time"))
		{
			ConfigTimeMenu();
		}

		//Window submenu
		if (ImGui::CollapsingHeader("Window"))
		{
			ConfigWindowMenu();
		}

		//Camera Submenu
		if (ImGui::CollapsingHeader("Camera"))
		{
			ConfigCameraMenu();
		}

		//Hardware specs of your own computer
		if (ImGui::CollapsingHeader("Hardware Specs"))
		{
			ConfigHardSpecsMenu();
		}

		//RENDERING OPTIONS
		if (ImGui::CollapsingHeader("Rendering"))
		{
			ConfigRenderingMenu();
		}

		//Bake quadtree options
		if (ImGui::CollapsingHeader("Bake Quadtree"))
		{
			bake_menu_open = true;
			ConfigBakeMenu();
		}
		else bake_menu_open = false;

		//Audio Options

		if (ImGui::CollapsingHeader("Audio"))
		{
			ConfigAudioMenu();
		}

		//Input Options
		if (ImGui::CollapsingHeader("Input"))
		{			
			ConfigInputMenu();
		}

		//Vram Usage
		if (ImGui::CollapsingHeader("VRAM Usage"))
		{
			ConfigVramUsageMenu();
		}
		ImGui::End();
	}
}


//Save and load window--------------------------------------------------------------
void ModuleEditor::ManageSaveWindow()
{
	if (save_window_open)
	{
		input_locked = true;
		ImGui::OpenPopup("Save current scene");
		ImGui::SetNextWindowSize(ImVec2(300, 100));
		ImGui::SetNextWindowPosCenter();

		if (ImGui::BeginPopupModal("Save current scene", nullptr))
		{
			static char scene_name[200];

			ImGui::InputText("Scene name", scene_name, IM_ARRAYSIZE(scene_name));

			if (ImGui::Button("Save") && strlen(scene_name) > 0)
			{
				App->scene->wants_to_save = true;
				save_window_open = false;
				App->scene->scene_name = scene_name;
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				save_window_open = false;
			}

		ImGui::EndPopup();
		}		

	}

	else if (load_window_open)
	{
		input_locked = true;
		ImGui::OpenPopup("Load scene");
		ImGui::SetNextWindowSize(ImVec2(200, 500));
		ImGui::SetNextWindowPosCenter();

		if(ImGui::BeginPopupModal("Load scene", nullptr))
		{
			ImGui::BeginChildFrame(0, ImVec2(150, 420));

			std::string path = App->fs->GetLibraryDirectory();
			path += "Scenes/";

			for (std::experimental::filesystem::recursive_directory_iterator::value_type it : std::experimental::filesystem::recursive_directory_iterator(path.c_str()))
			{
				std::string filename = std::experimental::filesystem::path(it.path().string().c_str()).stem().string().c_str();
				if (ImGui::Selectable(filename.c_str()))
				{
					App->scene->wants_to_load = true;
					App->scene->scene_name = filename;
					load_window_open = false;
				}
			}

			ImGui::EndChildFrame();

			if (ImGui::Button("Cancel"))
			{
				load_window_open = false;
			}
		}
		

		ImGui::EndPopup();
	}
}


//Hierarchy Window------------------------------------------------------------------------
void ModuleEditor::ManageHierarchyWindow()
{
	if (hierarchy_open)
	{
		ImGui::Begin("Hierarchy", &hierarchy_open, ImGuiWindowFlags_ShowBorders);

		if (ImGui::TreeNode("Scene"))
		{
			 std::vector<GameObject*> root_childrens = App->scene->root->GetChildrens();
						
			for (std::vector<GameObject*>::iterator it = root_childrens.begin(); it != root_childrens.end(); it++)
			{
				ManageHierarchyChildren(*(it));
			}
	
			ImGui::TreePop();
		}
		
		ImGui::End();
	}
}

void ModuleEditor::ManageHierarchyChildren(GameObject* object)
{

	bool open_element = ImGui::TreeNode(object->GetName());
	
	if (ImGui::IsItemClicked())
		SelectObject(object);

	if (open_element)
	{
		std::vector<GameObject*> childrens = object->GetChildrens();
		
		for (std::vector<GameObject*>::iterator it = childrens.begin(); it != childrens.end(); it++)
		{
			ManageHierarchyChildren(*(it));
		}

		ImGui::TreePop();
	}
	
}

//Inspector Window------------------------------------------------------------------------
void ModuleEditor::ManageInspectorWindow()
{
	if (inspector_open)
	{
		ImGui::Begin("Inspector", &inspector_open, ImGuiWindowFlags_ShowBorders);
		if(selected_object != nullptr)
			selected_object->OnEditor();
		ImGui::End();
	}
}

//Submenus of config window-------------------------------------------------------------

//Application submenu on Configuration window
void ModuleEditor::ConfigAppMenu()
{
	//Here are the text inputs and slider of the application submenu			
	static char str0[200];
	strcpy_s(str0, App->app_name.c_str());

	static char str1[200];
	strcpy_s(str1, App->org_name.c_str());

	if (ImGui::InputText("App Name", str0, IM_ARRAYSIZE(str0)))
	{
		App->app_name = str0;
		App->window->SetTitle(str0);
	}
	if (ImGui::InputText("Organization", str1, IM_ARRAYSIZE(str0)))
	{
		App->org_name = str1;
	}

	if (ImGui::Checkbox("Unlimited Framerate", &unlimitedFramerate))
	{
		if (unlimitedFramerate) App->SetMaxFrames(0);
		else
		{
			App->SetMaxFrames(max_fps);
		}
	};
	if (!unlimitedFramerate)
	{
		if (ImGui::SliderInt("Max FPS", &max_fps, 10, 60))
		{
			App->SetMaxFrames(max_fps);
		}
		ImGui::TextWrapped("Limit Framerate: ");
	}

	//Framerate Calculation
	if (calculate_fps_graph)
	{
		int frames;
		float milisec;
		App->GetFrames(frames, milisec);

		if (fps_log.size() > MAX_FPS_LOG)
		{
			for (int i = 1; i < fps_log.size(); i++)
			{
				fps_log[i - 1] = fps_log[i];
				ms_log[i - 1] = ms_log[i];
			}
			fps_log.pop_back();
			ms_log.pop_back();
		}


		fps_log.push_back(frames);
		ms_log.push_back(milisec);
		calculate_fps_graph = false;
	}
	//Plot the Histograms
	char title[25];
	sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
	ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
	sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
	ImGui::PlotHistogram("##Milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

}

//Window submenu on Configuration window
void ModuleEditor::ConfigWindowMenu()
{
	//Checkbox and sliders
	static bool vsync_active = App->window->vsync;
	static int width = 1080;
	static int height = 960;
	if (ImGui::Checkbox("Vsync Active", &vsync_active))
	{
		App->window->vsync = vsync_active;
		App->window->SwitchVSync();
	}
	//Width and Height control
	if (ImGui::SliderInt("Width", &width, 200, 3820))
	{
		App->window->window_width = width;
	}
	if (ImGui::SliderInt("Height", &height, 200, 2048))
	{
		App->window->window_height = height;
	}
	ImGui::TextColored({ 255,0,0,255 }, "Restart to apply resolution changes");

	//Window mode control
	if (ImGui::Button("Window Modes"))
		ImGui::OpenPopup("FilePopup");
	if (ImGui::BeginPopup("FilePopup"))
	{
		if (ImGui::MenuItem("Fullscreen"))
		{
			App->window->window_state = "fullscreen";
			SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN);
		}

		if (ImGui::MenuItem("Resizable"))
		{
			App->window->window_state = "resizable";
			SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_RESIZABLE);
		}
		if (ImGui::MenuItem("Full Desktop"))
		{
			App->window->window_state = "fullscreen_desktop";
			SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
		ImGui::EndPopup();
	}

	ImGui::Separator();
	//Change the background color of the ambient
	ImGui::PushItemWidth(140);
	static float color[4];
	if (ImGui::ColorPicker4("Background Color", color))
	{
		App->renderer3D->ChangeBackgroundColor({ color[0], color[1], color[2], color[3] });
	}
	ImGui::PopItemWidth();
}

//Camera submenu on Configuration window
void ModuleEditor::ConfigCameraMenu()
{
	//Camera speed
	float cam_vel = App->camera->camera_speed * 5000;
	ImGui::SliderFloat("Camera Speed", &cam_vel, 0, 200);
	App->camera->camera_speed = cam_vel / 5000;

	//Camera sensitivity
	float cam_sens = App->camera->camera_sensitivity * 1000;
	ImGui::SliderFloat("Camera Sensitivity", &cam_sens, 0, 100);
	App->camera->camera_sensitivity = cam_sens / 1000;

	//Camera position info
	ImGui::TextColored(ImVec4(255, 100, 0, 255), "Camera Position");
	ImGui::TextColored(ImVec4(100, 0, 255, 255), "X: %.3f   ", App->camera->frustum.pos.x);
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(100, 0, 255, 255), "Y: %.3f   ", App->camera->frustum.pos.y);
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(100, 0, 255, 255), "Z: %.3f", App->camera->frustum.pos.z);
}

//Hardware Specs submenu on Configuration window
void ModuleEditor::ConfigHardSpecsMenu()
{
	ImGui::TextWrapped("SDL Version: %i.%i.%i", App->system_specs.sdl_version.major, App->system_specs.sdl_version.minor, App->system_specs.sdl_version.patch);
	ImGui::TextWrapped("CPUs: %i", App->system_specs.cpus);
	ImGui::TextWrapped("System Ram: %f", App->system_specs.system_ram);
	ImGui::TextWrapped("CPU Cache: %i", App->system_specs.cpu_chache);
	
	ImGui::Separator();
	ImGui::TextColored({ 0, 255, 255, 255 }, "---------GPU Specs-----------");
	
	ImGui::TextColored({ 1, 1, 0, 1 }, "Vendor: %s", glGetString(GL_VENDOR));
	ImGui::TextColored({ 1, 1, 0, 1 }, "Renderer: %s", glGetString(GL_RENDERER));
	ImGui::TextColored({ 1, 1, 0, 1 }, "GL version: %s", glGetString(GL_VERSION));
	ImGui::TextColored({ 1, 1, 0, 1 }, "GL shading languaje version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	//To see which one of these your computer uses
	ImGui::Separator();
	ImGui::TextWrapped("Your hardware uses:");
	if (App->system_specs.altivec)
		ImGui::TextWrapped("AltiVec");
	if (App->system_specs.rdtsc)
		ImGui::TextWrapped("RDTSC");
	if (App->system_specs.mmx)
		ImGui::TextWrapped("MMX");
	if (App->system_specs.sse)
		ImGui::TextWrapped("SSE");
	if (App->system_specs.sse2)
		ImGui::TextWrapped("SSE2");
	if (App->system_specs.sse3)
		ImGui::TextWrapped("SSE3");
	if (App->system_specs.sse41)
		ImGui::TextWrapped("SSE41");
	if (App->system_specs.sse42)
		ImGui::TextWrapped("SSE42");
	if (App->system_specs.three_d_now)
		ImGui::TextWrapped("3DNow");
	if (App->system_specs.avx)
		ImGui::TextWrapped("AVX");
}

//Rendering submenu on Configuration window
void ModuleEditor::ConfigRenderingMenu()
{
	//Enable/Disable depth test
	if (ImGui::Checkbox("GL Depth", &App->renderer3D->gl_depth_enabled))
		(&App->renderer3D->gl_depth_enabled) ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);

	//Enable/Disable cull face
	if (ImGui::Checkbox("GL Cull Face", &App->renderer3D->gl_cull_face_enabled))
		(App->renderer3D->gl_cull_face_enabled) ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

	//Enable/Disable frustum culling
	ImGui::Checkbox("Frustum Culling", &App->renderer3D->frustum_culling);

	//Enable/Disable Lighting
	if (ImGui::Checkbox("GL Lighting", &App->renderer3D->gl_lighting_enabled))
		(App->renderer3D->gl_lighting_enabled) ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);

	//Pick the color of the light
	if (App->renderer3D->gl_lighting_enabled)
	{
		ImGui::Separator();
		ImGui::PushItemWidth(120);
		static float color[4];
		if (ImGui::ColorPicker4("Light Color", color));
		{
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, color);
		}
		ImGui::PopItemWidth();

		ImGui::Separator();
	}

	//Enable/Disable color material
	if (ImGui::Checkbox("GL Color Material", &App->renderer3D->gl_color_material_enabled))
		(App->renderer3D->gl_color_material_enabled) ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);

	//Enable/Disable 2D texture
	if (ImGui::Checkbox("GL Texture 2D", &App->renderer3D->gl_texture_2D_enabled))
		(App->renderer3D->gl_texture_2D_enabled) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

	//Enable/Disable wireframe mode
	if (ImGui::Checkbox("Wireframe mode", &App->renderer3D->gl_wireframe_enabled))
		(App->renderer3D->gl_wireframe_enabled) ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//Enable/Disable hard poly
	if (ImGui::Checkbox("Hard Poly", &App->renderer3D->hard_poly_enabled))
		(App->renderer3D->hard_poly_enabled) ? glShadeModel(GL_FLAT) : glShadeModel(GL_SMOOTH);

	
}

//Audio submenu on Configuration window
void ModuleEditor::ConfigAudioMenu()
{
	//Your audio driver
	ImGui::TextWrapped("Audio Driver:");
	ImGui::SameLine();
	ImGui::TextColored({ 255, 50, 100, 255 }, "%s", SDL_GetAudioDriver(App->audio->audio_driver));

	//The audio device you are using
	ImGui::TextWrapped("Audio Device:");
	ImGui::SameLine();
	ImGui::TextColored({ 255, 50, 100, 255 }, "%s", SDL_GetAudioDeviceName(App->audio->audio_device, 0));

	//Change the volume
	int volume = App->audio->GetVolume();
	if (ImGui::SliderInt("Volume", &volume, 0, 100))
	{
		App->audio->ChangeVolume(volume);
	}
}

//Input submenu on Configuration window
void ModuleEditor::ConfigInputMenu()
{
	//Mouse position on the window
	ImGui::TextWrapped("Mouse position:");
	ImGui::SameLine();
	ImGui::TextColored({ 255, 0, 100, 255 }, "X: %i ", App->input->GetMouseX());
	ImGui::SameLine();
	ImGui::TextColored({ 255, 0, 100, 255 }, "Y: %i", App->input->GetMouseY());
}

//Quadtree options
void ModuleEditor::ConfigBakeMenu()
{
	float X[2];
	float Y[2];
	float Z[2];

	X[0] = App->scene->scene_quadtree.root.box.minPoint.x;
	Y[0] = App->scene->scene_quadtree.root.box.minPoint.y;
	Z[0] = App->scene->scene_quadtree.root.box.minPoint.z;

	X[1] = App->scene->scene_quadtree.root.box.maxPoint.x;
	Y[1] = App->scene->scene_quadtree.root.box.maxPoint.y;
	Z[1] = App->scene->scene_quadtree.root.box.maxPoint.z;


	static float drag_speed = 0.1;
	if (ImGui::DragFloat2("X", X, drag_speed))
		App->scene->scene_quadtree.ResizeRoot(float3(X[0], Y[0], Z[0]), float3(X[1], Y[1], Z[1]));
	

	if (ImGui::DragFloat2("Y", Y, drag_speed))
		App->scene->scene_quadtree.ResizeRoot(float3(X[0], Y[0], Z[0]), float3(X[1], Y[1], Z[1]));

	if (ImGui::DragFloat2("Z", Z, drag_speed))
		App->scene->scene_quadtree.ResizeRoot(float3(X[0], Y[0], Z[0]), float3(X[1], Y[1], Z[1]));


	ImGui::DragInt("Max objects per node", &App->scene->scene_quadtree.max_game_objects, 1.0f, 1, 30);

	//Bake Quadtree
	if (ImGui::Button("Bake!"))
		App->scene->scene_quadtree.Calculate();
	
}

//VRAM Usage submenu on Configuration window
void ModuleEditor::ConfigVramUsageMenu()
{
	const GLubyte* v = glGetString(GL_VENDOR);
	char vendor[30];

	for (int i = 0; i<30; i++)
	{
		vendor[i] = v[i];
	}
	//Nvidia VRAM usage
	if (strcmp(vendor, "NVIDIA Corporation") == 0)
	{
		GLint total_mem_kb = 0;
		glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &total_mem_kb);

		GLint cur_avail_mem_kb = 0;
		glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &cur_avail_mem_kb);

		//Calculate VRAM usage
		char vram[25];
		vram_log.push_back(((float)total_mem_kb) / 1000.f);
		if (vram_log.size() > 100)
		{
			for (int i = 0; vram_log.size() > i - 1; i++)
			{
				vram_log[i] = vram_log[i + 1];
			}
			vram_log.pop_back();
		}
		sprintf_s(vram, 25, "VRAM usage: %.3f Mb", ((float)total_mem_kb) / 1000.f);
		ImGui::PlotHistogram("##VRAM usage: ", &vram_log[0], vram_log.size(), 0, vram, 0.0f, 100.0f, ImVec2(310, 100));

		//Calculate VRAM aviable
		char vram_curr[50];
		curr_vram_log.push_back(((float)cur_avail_mem_kb) / 1000.f);
		if (curr_vram_log.size() > 100)
		{
			for (int i = 0; curr_vram_log.size() > i - 1; i++)
			{
				curr_vram_log[i] = curr_vram_log[i + 1];
			}
			curr_vram_log.pop_back();
		}
		sprintf_s(vram_curr, 50, "VRAM Aviable: %.3f Mb", ((float)cur_avail_mem_kb) / 1000.f);
		ImGui::PlotHistogram("##VRAM Aviable: ", &curr_vram_log[0], curr_vram_log.size(), 0, vram_curr, 0.0f, 100.0f, ImVec2(310, 100));

	}
	//AMD VRAM usage still in project
	else if (strcmp(vendor, "AMD") == 0)
	{

	}
	else (ImGui::TextWrapped("VRam Usage only available for NVIDIA devices"));
}

//App time configuration
void ModuleEditor::ConfigTimeMenu()
{
	if (!App->IsAppRunning())
	{
		ImGui::TextWrapped("Game is stopped");
		return;
	}

	ImGui::TextWrapped("Game frame count: %i", App->FramesSinceStartUp());
	ImGui::TextWrapped("Time since game start: %.2f", App->RealTime()/1000);
	ImGui::TextWrapped("Time scale: %f", App->GetGameSpeed());
	ImGui::TextWrapped("Game dt: %.1f", App->GameDeltaTime());
	ImGui::TextWrapped("Real dt: %.1f", App->RealDeltaTime());
	ImGui::TextWrapped("Internal Game Seconds: %.2f", App->GameTime());
	
}


//Utility--------------------------------------------------------------------------------
void ModuleEditor::GetInputEvent(SDL_Event* e)
{
	ImGui_ImplSdlGL3_ProcessEvent(e);
}

bool ModuleEditor::SaveConfig(JSON_Object* config_data)
{
	LOG("Saving data to config--------");

	//Save renderer data
	json_object_dotset_boolean(config_data, "test_window_open", test_window_open);
	json_object_dotset_boolean(config_data, "console_open", console_open);
	json_object_dotset_boolean(config_data, "about_engine_open", about_engine_open);
	json_object_dotset_boolean(config_data, "hardware_open", hardware_open);
	json_object_dotset_boolean(config_data, "configuration_open", configuration_open);
	json_object_dotset_boolean(config_data, "inspector_open", inspector_open);
	json_object_dotset_boolean(config_data, "camera_config_open", camera_config_open);
	json_object_dotset_boolean(config_data, "hierarchy_open", hierarchy_open);

	return true;
}

void ModuleEditor::SelectObject(GameObject * g)
{
	objects_picked.clear();
	selected_object = g;
	objects_picked.push_back(g);
}



