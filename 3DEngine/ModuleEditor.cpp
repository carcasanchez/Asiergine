#include "Globals.h"
#include "ModuleEditor.h"
#include "Application.h"

#include "imgui/gl3w/GL/gl3w.h"
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

	gl3wInit();
	ImGui_ImplSdlGL3_Init(App->window->window);
	

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
	ImGui_ImplSdlGL3_Shutdown();
	return ret;
}
