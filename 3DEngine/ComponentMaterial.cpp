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
	if (texture_id != 0)
	{
		ImVec2 size;
		size.x = 200;
		size.y = 200;

		ImGui::Image((ImTextureID)texture_id, ImVec2(200, 200));
		ImGui::TextColored(ImVec4(0, 255, 0, 255), "%i x %i", (int)size.x, (int)size.y);
	}
}