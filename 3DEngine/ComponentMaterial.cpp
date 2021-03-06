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
		if (texture)
		{
			if (GetTextureID() != 0)
			{
				ImVec2 size;
				size.x = 200;
				size.y = 200;
				float2 texture_dimensions = texture->GetDimensions();

				ImGui::Image((ImTextureID)GetTextureID(), ImVec2(200, 200));
				ImGui::TextColored(ImVec4(0, 255, 0, 255), "%i x %i", (int)texture_dimensions.x, (int)texture_dimensions.y);
			}

	
			want_to_change_texture = ImGui::Button(GetTextureName().c_str());
			ImGui::TextWrapped("Instancies in scene: %i", texture->GetInstancies());
		}
		else
			want_to_change_texture = ImGui::Button("MISSING!");

		if (want_to_change_texture)
		{
			textures_to_show.clear();
			library_textures_path = App->fs->GetAssetDirectory();
			library_textures_path += "Textures/";
			ImGui::OpenPopup("Textures");
			want_to_change_texture = false;
			
			for (std::experimental::filesystem::recursive_directory_iterator::value_type it : std::experimental::filesystem::recursive_directory_iterator(library_textures_path.c_str()))
			{
				std::string filename = std::experimental::filesystem::path(it.path().string().c_str()).stem().string().c_str();
				std::string extension = std::experimental::filesystem::path(it.path().string().c_str()).extension().string().c_str();
				std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

				if (extension.compare(".dds") != 0 && extension.compare(".png") != 0 && extension.compare(".jpg") != 0 && extension.compare(".tga") != 0 && extension.compare(".jpeg") != 0)
					continue;

				textures_to_show.push_back(filename + extension);
			}
		}
		if (ImGui::BeginPopup("Textures"))
		{
			for (std::vector<std::string>::iterator it = textures_to_show.begin(); it != textures_to_show.end(); ++it)
			{
				if (ImGui::MenuItem((*it).c_str()))
				{
					library_textures_path += (*it);
					texture = (ResourceTexture*)App->resource_m->ChangeResource(texture, library_textures_path.c_str());
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
