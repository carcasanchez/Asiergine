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
		}

		if (texture)
		{
			want_to_change_texture = ImGui::Button(GetTextureName().c_str());
			ImGui::TextWrapped("Instancies in scene: %i", texture->GetInstancies());
		}
		else
			want_to_change_texture = ImGui::Button("MISSING!");

		if (want_to_change_texture)
		{
			library_textures_path = App->fs->GetAssetDirectory();
			library_textures_path += "Textures/";
			ImGui::OpenPopup("Textures");
			want_to_change_texture = false;
		}

		if (ImGui::BeginPopup("Textures"))
		{
			for (std::experimental::filesystem::recursive_directory_iterator::value_type it : std::experimental::filesystem::recursive_directory_iterator(library_textures_path.c_str()))
			{
				std::string filename = std::experimental::filesystem::path(it.path().string().c_str()).stem().string().c_str();
				std::string extension = std::experimental::filesystem::path(it.path().string().c_str()).extension().string().c_str();
				
				if (ImGui::MenuItem(filename.c_str()))
				{
					if (texture)
						texture->DecreaseInstancies();

					library_textures_path += filename + extension;
					texture = (ResourceTexture*)App->resource_m->LoadResource(library_textures_path.c_str());

					if (texture)
					{
						texture->IncreaseInstancies();
						name = filename;
					}
					else
						LOG("Texture %s does not exist!", filename.c_str());
				}
			}
			ImGui::EndPopup();
		}
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
