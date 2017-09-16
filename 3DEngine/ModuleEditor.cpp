#include "Globals.h"
#include "ModuleEditor.h"
#include "Application.h"


#include "Imgui/imgui_impl_sdl_gl3.h"


ModuleEditor::ModuleEditor(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	demo_window = NULL;
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
	bool ret = true;
	
	//ImGui::GetIO().DisplaySize.x = App->window->GetWindowPixels().x;
	//ImGui::GetIO().DisplaySize.y = App->window->GetWindowPixels().y;
	
	ImGui_ImplSdlGL3_Init(App->window->window);
	//ImGui::GetIO().Fonts->GetTexDataAsRGBA32();

	return ret;
}

bool ModuleEditor::Start()
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);

	return true;
}



update_status ModuleEditor::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{
	bool ret = true;
	ImGui::ShowTestWindow();

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::PostUpdate(float dt)
{
	ImGui::Render();
	return UPDATE_CONTINUE;
}

bool ModuleEditor::CleanUp()
{
	bool ret = true;
	ImGui::End();
	return ret;
}
