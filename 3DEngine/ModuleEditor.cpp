#include "Globals.h"
#include "ModuleEditor.h"
#include "Application.h"
#include "Math.h"


#include "Imgui/imgui_impl_sdl_gl3.h"


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
	

	return ret;
}

bool ModuleEditor::Start()
{
	bool ret = true;
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	return ret;
}



update_status ModuleEditor::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	return ret;
}

update_status ModuleEditor::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;
	
	ManageMainMenuBar();

	ManageAboutWindow();

	ManageConsole();
	ManageConfigurationWindow();
	ManageExampleWindow();
	ManageHierarchyWindow();
	ManageInspectorWindow();
	ManageProfilerWindow();

			
	return ret;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;
	
	ImGui::Render();

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





//MAIN MENU BAR management
void ModuleEditor::ManageMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{

		if (ImGui::BeginMenu("File"))
		{			
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			Window_option();
			ImGui::EndMenu();
		}
				
		if (ImGui::BeginMenu("Exit Editor"))
		{
			ExitEditor_option();
			ImGui::EndMenu();
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

	if (ImGui::MenuItem("Profiler"))
		profiler_open = true;

	if (ImGui::MenuItem("About the engine"))
		about_engine_open = true;


	
}

void ModuleEditor::ExitEditor_option()
{
	if (ImGui::MenuItem("Exit"))
	{
		quit_editor = true;
	}
}




//EMERGENT WINDOWS management

//Example window management
void ModuleEditor::ManageExampleWindow()
{
	if (test_window_open == true)
		ImGui::ShowTestWindow();
}

//About window
void ModuleEditor::ManageAboutWindow()
{
	if (about_engine_open)
	{
		ImGui::Begin("About", &about_engine_open);
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



//Console management
void ModuleEditor::ManageConsole()
{
	if(console_open)
		console.Draw("Asiergine Console", &console_open);
}

void ModuleEditor::DrawInConsole(const char * to_console)
{
		console.AddLog(to_console);
}


//Configuration window
void ModuleEditor::ManageConfigurationWindow()
{
	if (configuration_open)
	{
		ImGui::Begin("Configuration", &configuration_open);
		//Options menu inside configuration window
		if (ImGui::BeginMenu("Options"))
		{
			ImGui::MenuItem("App");
			ImGui::EndMenu();
		}
		//Application submenu
		if (ImGui::CollapsingHeader("Application"))
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
			
			static int max_fps = 0;
			if (ImGui::SliderInt("Max FPS", &max_fps, 0, 60))
			{
				App->SetMaxFrames(max_fps);
			}
			ImGui::TextWrapped("Limit Framerate: ");
			

			//Framerate Calculation
			
			int frames;
			float milisec;
			App->GetFrames(frames, milisec);

			if (fps_log.size() > 100)
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

			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
			ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
			ImGui::PlotHistogram("##Milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

		}
		//Window submenu
		if (ImGui::CollapsingHeader("Window"))
		{
			//Checkbox and sliders
			static bool active = true;
			static float brightness = 1.0f;
			static int width = 0;
			static int height = 0;
			if (ImGui::Checkbox("Vsync Active", &active))
			{
				App->window->vsync = true;
			}
			else
				App->window->vsync = false;

			ImGui::TextWrapped("Icon:  *default*");
			ImGui::SliderFloat("Brightness", &brightness, 0, 1);
			ImGui::SliderInt("Width", &width, 0, 3820);
			ImGui::SliderInt("Height", &height, 0, 2048);
			ImGui::TextWrapped("Refresh Rate: ");
			
			if (ImGui::Checkbox("Fullscreen", &fullscreen)) 
			{				
				if (fullscreen)
				{
					SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN);
					App->window->fullscreen = true;
				}
				
				else
				{
					SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_RESIZABLE);
					App->window->fullscreen = false;
				}
			}

			ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", &resizable)) 
			{
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Restart to apply");
				if (resizable)
				{
					SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_RESIZABLE);
					App->window->resizable = true;
				}
				else 
				{
					App->window->resizable = false;
				}
			}

			if (ImGui::Checkbox("Borderless", &borderless))
			{
				if (borderless)
				{
					SDL_SetWindowBordered(App->window->window, bordered);
					App->window->borderless = true;
				}
				else 
				{
					SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_RESIZABLE);
					SDL_SetWindowBordered(App->window->window, SDL_FALSE);
					App->window->borderless = false;
				}
			}
				
			ImGui::SameLine();
			if (ImGui::Checkbox("Full Desktop", &fulldesktop))
			{
				if (fulldesktop)
				{
					SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
					App->window->fullscreen_desktop = true;
				}
				else
				{
					SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_RESIZABLE);
					App->window->fullscreen_desktop = false;
				}
			}
		}
		//Hardware specs of your own computer
		if (ImGui::CollapsingHeader("Hardware Specs"))
		{
			ImGui::TextWrapped("SDL Version: %i.%i.%i", App->system_specs.sdl_version.major, App->system_specs.sdl_version.minor, App->system_specs.sdl_version.patch);
			ImGui::TextWrapped("CPUs: %i", App->system_specs.cpus);
			ImGui::TextWrapped("System Ram: %f", App->system_specs.system_ram);
			ImGui::TextWrapped("CPU Cache: %i", App->system_specs.cpu_chache);
			//TODO:   glGetString(GL_VERSION)

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

		ImGui::End();
	}
}

//Hierarchy window
void ModuleEditor::ManageHierarchyWindow()
{
	if (hierarchy_open)
	{
		ImGui::Begin("Hierarchy", &hierarchy_open);
		ImGui::MenuItem("GameObject1");
		ImGui::MenuItem("GameObject2");
		ImGui::MenuItem("GameObject3");
		ImGui::End();
	}
}

//Inspector Window
void ModuleEditor::ManageInspectorWindow()
{
	if (inspector_open)
	{
		ImGui::Begin("Inspector", &inspector_open);
		ImGui::MenuItem("GameObject1");
		ImGui::MenuItem("GameObject2");
		ImGui::MenuItem("GameObject3");
		ImGui::End();
	}
}

void ModuleEditor::ManageProfilerWindow()
{
	if (profiler_open)
	{
		ImGui::Begin("Profiler", &profiler_open);
		
		std::list<Module*>::iterator it = App->list_modules.begin();

		for (;it!=App->list_modules.end();it++)
		{
			if (ImGui::CollapsingHeader((*it)->GetModuleName().c_str()))
			{
				ImGui::TextWrapped("Start Time: %f ms", (*it)->start_profiler.last_check_ms);
				ImGui::TextWrapped("PreUpdate Time: %f ms", (*it)->pre_update_profiler.last_check_ms);
				ImGui::TextWrapped("Update Time: %f ms", (*it)->update_profiler.last_check_ms);
				ImGui::TextWrapped("PostUpdate Time: %f ms", (*it)->post_update_profiler.last_check_ms);
			}
		}

		ImGui::End();
	}
}



void ModuleEditor::GetInputEvent(SDL_Event* e)
{
	ImGui_ImplSdlGL3_ProcessEvent(e);
}




