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
	ImGui::Begin("Window");
	return ret;
}

bool ModuleEditor::Update()
{
	bool ret = true;
	ImGui::ShowTestWindow();
	return ret;
}

bool ModuleEditor::CleanUp()
{
	bool ret = true;
	ImGui::End();
	return ret;
}
