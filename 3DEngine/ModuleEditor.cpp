#include "Globals.h"
#include "ModuleEditor.h"
#include "Application.h"
#include "Math.h"


#include "Imgui/imgui_impl_sdl_gl3.h"


ModuleEditor::ModuleEditor(Application * app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
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
	ManageConsole();
	ManageExampleWindow();

	GenerateRandomCollision();
		
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



//Main menu bar management
void ModuleEditor::ManageMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Example Window"))
		{
			ShowExampleWindow_option();
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

void ModuleEditor::ExitEditor_option()
{
	if (ImGui::MenuItem("Exit"))
	{
		quit_editor = true;
	}
}


//Example window management
void ModuleEditor::ManageExampleWindow()
{
	if (test_window_open == true)
		ImGui::ShowTestWindow();
}



//Console management
void ModuleEditor::ManageConsole()
{
	static AppConsole console;
	console.Draw("Example: Console", &console_open);
}



//Collision simulation
void ModuleEditor::GenerateRandomCollision()
{
	LCG rand_generator;

	float ray_pos_x = rand_generator.Int(0, 10);
	float ray_pos_y = rand_generator.Int(0, 10);
	float ray_pos_z = rand_generator.Int(0, 10);
	ray.pos.Set(ray_pos_x, ray_pos_y, ray_pos_z);

	float ray_dir_x = rand_generator.Int(0, 10);
	float ray_dir_y = rand_generator.Int(0, 10);
	float ray_dir_z = rand_generator.Int(0, 10);
	ray.dir.Set(ray_dir_x, ray_dir_y, ray_dir_z);


	float sphere_pos_x = rand_generator.Int(0, 10);
	float sphere_pos_y = rand_generator.Int(0, 10);
	float sphere_pos_z = rand_generator.Int(0, 10);
	sphere.pos.Set(sphere_pos_x, sphere_pos_y, sphere_pos_z);
	sphere.r = rand_generator.Int(0, 6);



	bool inter = ray.Intersects(sphere);

}
