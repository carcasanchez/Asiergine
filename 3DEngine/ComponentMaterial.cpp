#include "ComponentMaterial.h"
#include "Application.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include "Globals.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "mmgr\mmgr.h"
#include "imgui\imgui.h"


ComponentMaterial::ComponentMaterial(GameObject* g):Component(g)
{
	type = COMPONENT_MATERIAL;
}


ComponentMaterial::~ComponentMaterial()
{
	if (texture)
		texture->DecreaseInstancies();
}

void ComponentMaterial::OnEditor()
{
	if (IsActive() == true)
	{
		if (GetTextureID() != 0)
		{
			ImVec2 size;
			size.x = 200;
			size.y = 200;

			ImGui::Image((ImTextureID)GetTextureID(), ImVec2(200, 200));
			ImGui::TextColored(ImVec4(0, 255, 0, 255), "%i x %i", (int)size.x, (int)size.y);
			ImGui::TextWrapped("%s", texture->GetTextureName().c_str());	
		}

		if (texture)
		{
			ImGui::TextWrapped("Instancies: %i", texture->GetInstancies());
		}

		/*if (ImGui::Button("Change Texture"))
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
						texture->SetData(App->importer->LoadTexture(path.c_str(), true));
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
		}*/
	}
}

uint ComponentMaterial::PrepareToSave() const
{
	uint size = 0;

	size += sizeof(uint); // Length of texture name length
	size += GetTextureName().size(); // Length of texture name
	return size;
}

void ComponentMaterial::Save(char *& cursor) const
{
	uint texture_name_size = GetTextureName().size();
	uint size_of = sizeof(uint);
	memcpy(cursor, &texture_name_size, size_of);
	cursor += size_of;

	std::string text_name = GetTextureName();

	size_of = texture_name_size;
	memcpy(cursor, text_name.data(), size_of);
	cursor += size_of;
}
