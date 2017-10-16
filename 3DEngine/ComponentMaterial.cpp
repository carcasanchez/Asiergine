#include "ComponentMaterial.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "mmgr\mmgr.h"


ComponentMaterial::ComponentMaterial(GameObject* g, uint id):Component(g), texture_id(id)
{
	type = COMPONENT_MATERIAL;
}


ComponentMaterial::~ComponentMaterial()
{
	glDeleteTextures(1, &texture_id);
}
