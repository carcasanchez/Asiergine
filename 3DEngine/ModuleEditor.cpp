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



//Console management
void ModuleEditor::ManageConsole()
{
	static AppConsole console;
	console.Draw("Example: Console", &console_open);
}

ModuleEditor::AppConsole::AppConsole()
{
	ClearLog();
	memset(InputBuf, 0, sizeof(InputBuf));
	HistoryPos = -1;
	Commands.push_back("HELP");
	Commands.push_back("HISTORY");
	Commands.push_back("CLEAR");
	Commands.push_back("CLASSIFY");  // "classify" is here to provide an example of "C"+[tab] completing to "CL" and displaying matches.
	AddLog("Welcome to Asiergine!");
}

ModuleEditor::AppConsole::~AppConsole()
{
	ClearLog();
	for (int i = 0; i < History.Size; i++)
		free(History[i]);
}

void ModuleEditor::AppConsole::ClearLog()
{
	for (int i = 0; i < Items.Size; i++)
		free(Items[i]);
	Items.clear();
	ScrollToBottom = true;
}

void ModuleEditor::AppConsole::Draw(const char* title, bool* p_open)
{
	{
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin(title, p_open))
		{
			ImGui::End();
			return;
		}

		//ImGui::TextWrapped("Enter 'HELP' for help, press TAB to use text completion.");

		// TODO: display items starting from the bottom

		if (ImGui::SmallButton("Collision checking")) { AddLog("%d some text", Items.Size); AddLog("some more text"); AddLog("display very important message here!"); } ImGui::SameLine();
		if (ImGui::SmallButton("Clear")) { ClearLog(); } ImGui::SameLine();
		bool copy_to_clipboard = ImGui::SmallButton("Copy"); ImGui::SameLine();
		if (ImGui::SmallButton("Scroll to bottom")) ScrollToBottom = true;
		//static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

		ImGui::Separator();



		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::Selectable("Clear")) ClearLog();
			ImGui::EndPopup();
		}

		// Display every line as a separate entry so we can change their color or add custom widgets. If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
		// NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping to only process visible items.
		// You can seek and display only the lines that are visible using the ImGuiListClipper helper, if your elements are evenly spaced and you have cheap random access to the elements.
		// To use the clipper we could replace the 'for (int i = 0; i < Items.Size; i++)' loop with:
		//     ImGuiListClipper clipper(Items.Size);
		//     while (clipper.Step())
		//         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		// However take note that you can not use this code as is if a filter is active because it breaks the 'cheap random-access' property. We would need random-access on the post-filtered list.
		// A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices that passed the filtering test, recomputing this array when user changes the filter,
		// and appending newly elements as they are inserted. This is left as a task to the user until we can manage to improve this example code!
		// If your items are of variable size you may want to implement code similar to what ImGuiListClipper does. Or split your data into fixed height items to allow random-seeking into your list.
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
		if (copy_to_clipboard)
			ImGui::LogToClipboard();
		for (int i = 0; i < Items.Size; i++)
		{
			const char* item = Items[i];
			ImVec4 col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // A better implementation may store a type per-item. For the sample let's just parse the text.
			if (strstr(item, "[error]")) col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
			else if (strncmp(item, "# ", 2) == 0) col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_Text, col);
			ImGui::TextUnformatted(item);
			ImGui::PopStyleColor();
		}
		if (copy_to_clipboard)
			ImGui::LogFinish();
		if (ScrollToBottom)
			ImGui::SetScrollHere();
		ScrollToBottom = false;
		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::Separator();

		// Command-line
		if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void*)this))
		{
			char* input_end = InputBuf + strlen(InputBuf);
			while (input_end > InputBuf && input_end[-1] == ' ') { input_end--; } *input_end = 0;
			if (InputBuf[0])
				ExecCommand(InputBuf);
			strcpy(InputBuf, "");
		}

		// Demonstrate keeping auto focus on the input box
		if (ImGui::IsItemHovered() || (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
			ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

		ImGui::End();
	}
}

