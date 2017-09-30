#include "Globals.h"
#include "ModuleEditor.h"
#include "Application.h"
#include "Math.h"
#include "Primitive.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

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

	//reserve for fps_log
	fps_log = std::vector<float>(MAX_FPS_LOG, 0);
	ms_log = std::vector<float>(MAX_FPS_LOG, 0);



	//------Cube 2
	//---1
	vertex[0] = 20.f;
	vertex[1] = 20.f;
	vertex[2] = 30.f;

	vertex[3] = 20.f;
	vertex[4] = 20.f;
	vertex[5] = 20.f;

	vertex[6] = 30.f;
	vertex[7] = 20.f;
	vertex[8] = 20.f;
	
	//-----2
	vertex[9] = 30.f;
	vertex[10] = 20.f;
	vertex[11] = 30.f;

	vertex[12] = 20.f;
	vertex[13] = 20.f;
	vertex[14] = 30.f;

	vertex[15] = 30.f;
	vertex[16] = 20.f;
	vertex[17] = 20.f;

	//------3
	vertex[18] = 30.f;
	vertex[19] = 20.f;
	vertex[20] = 30.f;

	vertex[21] = 30.f;
	vertex[22] = 20.f;
	vertex[23] = 20.f;

	vertex[24] = 30.f;
	vertex[25] = 30.f;
	vertex[26] = 20.f;

	//-----4
	vertex[27] = 30.f;
	vertex[28] = 30.f;
	vertex[29] = 30.f;

	vertex[30] = 30.f;
	vertex[31] = 20.f;
	vertex[32] = 30.f;

	vertex[33] = 30.f;
	vertex[34] = 30.f;
	vertex[35] = 20.f;

	//---------------5
	vertex[36] = 30.f;
	vertex[37] = 20.f;
	vertex[38] = 30.f;

	vertex[39] = 30.f;
	vertex[40] = 30.f;
	vertex[41] = 30.f;

	vertex[42] = 20.f;
	vertex[43] = 30.f;
	vertex[44] = 30.f;


	//--------------6
	vertex[45] = 20.f;
	vertex[46] = 20.f;
	vertex[47] = 30.f;

	vertex[48] = 30.f;
	vertex[49] = 20.f;
	vertex[50] = 30.f;

	vertex[51] = 20.f;
	vertex[52] = 30.f;
	vertex[53] = 30.f;

	//---------------7
	vertex[54] = 30.f;
	vertex[55] = 30.f;
	vertex[56] = 30.f;

	vertex[57] = 30.f;
	vertex[58] = 30.f;
	vertex[59] = 20.f;

	vertex[60] = 20.f;
	vertex[61] = 30.f;
	vertex[62] = 20.f;

	//---------------8
	vertex[63] = 20.f;
	vertex[64] = 30.f;
	vertex[65] = 30.f;

	vertex[66] = 30.f;
	vertex[67] = 30.f;
	vertex[68] = 30.f;

	vertex[69] = 20.f;
	vertex[70] = 30.f;
	vertex[71] = 20.f;

	//---------------9
	vertex[72] = 20.f;
	vertex[73] = 30.f;
	vertex[74] = 30.f;

	vertex[75] = 20.f;
	vertex[76] = 30.f;
	vertex[77] = 20.f;

	vertex[78] = 20.f;
	vertex[79] = 20.f;
	vertex[80] = 20.f;


	//-----------10
	vertex[81] = 20.f;
	vertex[82] = 20.f;
	vertex[83] = 30.f;

	vertex[84] = 20.f;
	vertex[85] = 30.f;
	vertex[86] = 30.f;

	vertex[87] = 20.f;
	vertex[88] = 20.f;
	vertex[89] = 20.f;

	//-----------11	
	vertex[90] = 20.f;
	vertex[91] = 20.f;
	vertex[92] = 20.f;

	vertex[93] = 20.f;
	vertex[94] = 30.f;
	vertex[95] = 20.f;

	vertex[96] = 30.f;
	vertex[97] = 30.f;
	vertex[98] = 20.f;

	//-----------12
	vertex[99] = 30.f;
	vertex[100] = 20.f;
	vertex[101] = 20.f;

	vertex[102] = 20.f;
	vertex[103] = 20.f;
	vertex[104] = 20.f;

	vertex[105] = 30.f;
	vertex[106] = 30.f;
	vertex[107] = 20.f;


	//alloc vertex
	num_of_vertex = 39;
	glGenBuffers(1, (GLuint*)&(array_id));
	glBindBuffer(GL_ARRAY_BUFFER, array_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* num_of_vertex * 3, vertex, GL_STATIC_DRAW);

	//alloc indices
	glGenBuffers(1, (GLuint*)&(array_id2));
	glBindBuffer(GL_ARRAY_BUFFER, array_id2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 8 * 3, vertex2, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*36*4, indices, GL_STATIC_DRAW);


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
	



			
	return ret;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;
	
	glLineWidth(2.0f);


	//Direct mode drawing
	glBegin(GL_TRIANGLES);
	//1
	glVertex3f(0.f, 0.f, 10.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(10.f, 0.f, 0.f);
	//2
	glVertex3f(10.f, 0.f, 10.f);
	glVertex3f(0.f, 0.f, 10.f);
	glVertex3f(10.f, 0.f, 0.f);
	//3
	glVertex3f(10.f, 0.f, 10.f);
	glVertex3f(10.f, 0.f, 0.f);
	glVertex3f(10.f, 10.f, 0.f);
	//4
	glVertex3f(10.f, 10.f, 10.f);
	glVertex3f(10.f, 0.f, 10.f);
	glVertex3f(10.f, 10.f, 0.f);
	//5
	glVertex3f(10.f, 0.f, 10.f);
	glVertex3f(10.f, 10.f, 10.f);
	glVertex3f(0.f, 10.f, 10.f);
	//6
	glVertex3f(0.f, 0.f, 10.f);
	glVertex3f(10.f, 0.f, 10.f);
	glVertex3f(0.f, 10.f, 10.f);
	//7
	glVertex3f(10.f, 10.f, 10.f);
	glVertex3f(10.f, 10.f, 0.f);
	glVertex3f(0.f, 10.f, 0.f);
	//8
	glVertex3f(0.f, 10.f, 10.f);
	glVertex3f(10.f, 10.f, 10.f);
	glVertex3f(0.f, 10.f, 0.f);
	//9
	glVertex3f(0.f, 10.f, 10.f);
	glVertex3f(0.f, 10.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	//10
	glVertex3f(0.f, 0.f, 10.f);
	glVertex3f(0.f, 10.f, 10.f);
	glVertex3f(0.f, 0.f, 0.f);
	//11
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 10.f, 0.f);
	glVertex3f(10.f, 10.f, 0.f);
	//12
	glVertex3f(10.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(10.f, 10.f, 0.f);
	glEnd();

	

	//Array drawing

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, array_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	// ... draw other buffers
	glDrawArrays(GL_TRIANGLES, 0, num_of_vertex * 3);
	glDisableClientState(GL_VERTEX_ARRAY);
	



	//Index drawing

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, array_id2);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);




	glEnd();
	glLineWidth(1.0f);

	Plane_prim plane;
	plane.Render();


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

void ModuleEditor::DrawUI()
{
	ManageMainMenuBar();
	ManageAboutWindow();
	ManageConsole();
	ManageConfigurationWindow();
	ManageExampleWindow();
	ManageHierarchyWindow();
	ManageInspectorWindow();

	ImGui::Render();
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
		if (ImGui::Button("Parson"))
			ShellExecute(NULL, "open", "https://github.com/kgabis/parson", NULL, NULL, SW_SHOWNORMAL);



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
			static bool vsync_active = App->window->vsync;
			static int width = 1080;
			static int height = 960;
			if (ImGui::Checkbox("Vsync Active", &vsync_active))
			{
				App->window->vsync = vsync_active;
				App->window->SwitchVSync();
			}
			
			if (ImGui::SliderInt("Width", &width, 200, 3820))
			{
				App->window->window_width = width;
			}
			if (ImGui::SliderInt("Height", &height, 200, 2048))
			{
				App->window->window_height = height;
			}
			ImGui::TextColored({ 255,0,0,255 }, "Restart to apply resolution changes");

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

			ImGui::PushItemWidth(140);
			static float color[4];
			if (ImGui::ColorPicker4("Background Color", color))
			{
				App->renderer3D->ChangeBackgroundColor({color[0], color[1], color[2], color[3]});
			}
			ImGui::PopItemWidth();
		}

		//Hardware specs of your own computer
		if (ImGui::CollapsingHeader("Hardware Specs"))
		{
			ImGui::TextWrapped("SDL Version: %i.%i.%i", App->system_specs.sdl_version.major, App->system_specs.sdl_version.minor, App->system_specs.sdl_version.patch);
			ImGui::TextWrapped("CPUs: %i", App->system_specs.cpus);
			ImGui::TextWrapped("System Ram: %f", App->system_specs.system_ram);
			ImGui::TextWrapped("CPU Cache: %i", App->system_specs.cpu_chache);


			ImGui::Separator();
			ImGui::TextColored({ 0, 255, 255, 255 }, "---------GPU Specs-----------");


			ImGui::TextColored({ 255, 0, 255, 255 }, "Vendor: %s", glGetString(GL_VENDOR));
			ImGui::TextColored({ 255, 0, 255, 255 }, "Renderer: %s", glGetString(GL_RENDERER));
			ImGui::TextColored({255, 0, 255, 255}, "GL version: %s", glGetString(GL_VERSION));
			ImGui::TextColored({ 255, 0, 255, 255 }, "GL shading languaje version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));


			

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

		//RENDERING OPTIONS
		if (ImGui::CollapsingHeader("Rendering"))
		{
		
			if (ImGui::Checkbox("GL Depth", &App->renderer3D->gl_depth_enabled))
				(&App->renderer3D->gl_depth_enabled) ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);

			if (ImGui::Checkbox("GL Cull Face", &App->renderer3D->gl_cull_face_enabled))
				(App->renderer3D->gl_cull_face_enabled) ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

			if (ImGui::Checkbox("GL Lighting", &App->renderer3D->gl_lighting_enabled))	
				(App->renderer3D->gl_lighting_enabled) ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);

			if (App->renderer3D->gl_lighting_enabled)
			{
				ImGui::Separator();
				ImGui::PushItemWidth(120);
				static float color[4];
				if(ImGui::ColorPicker4("Light Color", color));
				{
					glLightModelfv(GL_LIGHT_MODEL_AMBIENT, color);
				}
				ImGui::PopItemWidth();

				ImGui::Separator();
			}
			

			if (ImGui::Checkbox("GL Color Material", &App->renderer3D->gl_color_material_enabled))
				(App->renderer3D->gl_color_material_enabled) ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);

			 if (ImGui::Checkbox("GL Texture 2D", &App->renderer3D->gl_texture_2D_enabled))
				(App->renderer3D->gl_texture_2D_enabled) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
						
			 if (ImGui::Checkbox("Wireframe mode", &App->renderer3D->gl_wireframe_enabled))
				(App->renderer3D->gl_wireframe_enabled) ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
									
			 if(ImGui::Checkbox("Hard Poly", &App->renderer3D->hard_poly_enabled))
				(App->renderer3D->hard_poly_enabled) ? glShadeModel(GL_FLAT) : glShadeModel(GL_SMOOTH);
		}

		//Audio Options

		if (ImGui::CollapsingHeader("Audio"))
		{
			ImGui::TextWrapped("Audio Driver:");
			ImGui::SameLine();
			ImGui::TextColored({255, 50, 100, 255},"%s", SDL_GetAudioDriver(App->audio->audio_driver));

			ImGui::TextWrapped("Audio Device:");
			ImGui::SameLine();
			ImGui::TextColored({ 255, 50, 100, 255 }, "%s", SDL_GetAudioDeviceName(App->audio->audio_device, 0));

			ImGui::SliderInt("Volume", &App->audio->volume, 0, 100);
		}

		//Input Options
		if (ImGui::CollapsingHeader("Input"))
		{			
			ImGui::TextWrapped("Mouse position:");
			ImGui::SameLine();
			ImGui::TextColored({ 255, 0, 100, 255 }, "X: %i ", App->input->GetMouseX());
			ImGui::SameLine();
			ImGui::TextColored({ 255, 0, 100, 255 }, "Y: %i", App->input->GetMouseY());
		/*	GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST));
			pRawInputDeviceList = (RAWINPUTDEVICELIST*) malloc(sizeof(RAWINPUTDEVICELIST) * nDevices);

			ImGui::TextWrapped("Num of Devices: %i", GetRawInputDeviceList(pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST)));

			free(pRawInputDeviceList);*/

		}

		//Vram Usage
		if (ImGui::CollapsingHeader("VRAM Usage"))
		{

			const GLubyte* v = glGetString(GL_VENDOR);
			char vendor[30];

			for (int i = 0; i<30; i++)
			{
				vendor[i] = v[i];
			}

			if (strcmp(vendor, "NVIDIA Corporation") == 0)
			{
				GLint total_mem_kb = 0;
				glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
					&total_mem_kb);

				GLint cur_avail_mem_kb = 0;
				glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
					&cur_avail_mem_kb);

				char vram[25];
				vram_log.push_back(((float)total_mem_kb) / 1000.f);
				if (vram_log.size() > 100)
				{
					for (int i = 0; vram_log.size() > i-1; i++)
					{
						vram_log[i] = vram_log[i + 1];
					}
					vram_log.pop_back();
				}
				sprintf_s(vram, 25, "VRAM usage: %.3f Mb", ((float)total_mem_kb) / 1000.f);
				ImGui::PlotHistogram("##VRAM usage: ", &vram_log[0], vram_log.size(), 0, vram, 0.0f, 100.0f, ImVec2(310, 100));

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
			else (ImGui::TextWrapped("VRam Usage only available for NVIDIA devices"));

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


void ModuleEditor::GetInputEvent(SDL_Event* e)
{
	ImGui_ImplSdlGL3_ProcessEvent(e);
}




