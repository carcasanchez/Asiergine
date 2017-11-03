#include "ComponentMaterial.h"
#include "Application.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include "Globals.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "mmgr\mmgr.h"
#include "imgui\imgui.h"


ComponentMaterial::ComponentMaterial(GameObject* g, uint id):Component(g), texture_id(id)
{
	type = COMPONENT_MATERIAL;
}


ComponentMaterial::~ComponentMaterial()
{
	glDeleteTextures(1, &texture_id);
}

void ComponentMaterial::OnEditor()
{
	if (IsActive() == true)
	{
		if (texture_id != 0)
		{
			ImVec2 size;
			size.x = 200;
			size.y = 200;

			ImGui::Image((ImTextureID)texture_id, ImVec2(200, 200));
			ImGui::TextColored(ImVec4(0, 255, 0, 255), "%i x %i", (int)size.x, (int)size.y);
			ImGui::TextWrapped("%s", texture_name.c_str());	
		}


		if (ImGui::Button("Change Texture"))
		{
			change_text_window = true;
		}

		if (change_text_window)
		{
			ImGui::OpenPopup("Change Texture");
			ImGui::SetNextWindowSize(ImVec2(500, 500));
			ImGui::SetNextWindowPosCenter();

			if (ImGui::BeginPopupModal("Change Texture", nullptr))
			{
				ImGui::BeginChildFrame(0, ImVec2(150, 420));

				std::string path = App->fs->GetLibraryDirectory();
				path += "Textures/";

				for (std::experimental::filesystem::recursive_directory_iterator::value_type it : std::experimental::filesystem::recursive_directory_iterator(path.c_str()))
				{
					std::string filename = std::experimental::filesystem::path(it.path().string().c_str()).stem().string().c_str();
					if (ImGui::Selectable(filename.c_str()))
					{
						path += filename.c_str();
						path += ".dds";
						texture_id = App->importer->LoadTexture(path.c_str(), true);
						texture_name = filename;
						change_text_window = false;
					}
				}

				ImGui::EndChildFrame();

				if (ImGui::Button("Cancel"))
				{
					change_text_window = false;
				}

				ImGui::EndPopup();
			}
		}
	}
}