#include "Globals.h"
#include "ModuleEditor.h"
#include "Application.h"


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
	ShowExampleAppMainMenuBar();

	if(test_window_open == true)
	ImGui::ShowTestWindow();

	if (quit_editor)
		ret = UPDATE_STOP;

	return ret;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;
	ImGui::Render();
	return ret;
}

bool ModuleEditor::CleanUp()
{
	bool ret = true;
	ImGui_ImplSdlGL3_Shutdown();
	return ret;
}



void ModuleEditor::ShowExampleAppMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Example Window"))
		{
			ShowExampleWindow();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Close"))
		{
			quit_editor = true;
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void ModuleEditor::ShowExampleWindow()
{
	ImGui::MenuItem("Open Demo", NULL, false, false);
	if (ImGui::MenuItem("Show")) 
	{
		test_window_open = true;
		ImGui::ShowTestWindow();
	}	
}

void ModuleEditor::ShowQuitWindow()
{

}