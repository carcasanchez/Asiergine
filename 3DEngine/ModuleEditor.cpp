#include "Globals.h"
#include "ModuleEditor.h"
#include "Application.h"


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
	
	ImGui::GetIO().DisplaySize.x = App->window->GetWindowPixels().x;
	ImGui::GetIO().DisplaySize.y = App->window->GetWindowPixels().y;

	//ImGui::GetIO().Fonts->GetTexDataAsRGBA32();

	return ret;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	ImGui::NewFrame();
	ImGui::Begin("Window");
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
