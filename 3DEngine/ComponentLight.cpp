#include "ComponentLight.h"
#include "Application.h"
#include "Glew/include/glew.h"
#include "SDL/include/SDL_opengl.h"
#include "Globals.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "imgui\imgui.h"
#include "CompTransform.h"
#include "GameObject.h"
#include "ModuleRenderer3D.h"


ComponentLight::ComponentLight(GameObject* g, Light l) :Component(g)
{
	type = COMPONENT_LIGHT;
	light = l;
	name = "Light";
}


ComponentLight::~ComponentLight()
{
	App->renderer3D->light_index--;
}

void ComponentLight::Update(float real_dt, float game_dt)
{
	if (active)
	{
		CompTransform* transf = (CompTransform*)game_object->GetComponentByType(COMPONENT_TRANSFORM);
		if (transf)
		{
			float3 pos = transf->GetTranslation();
			light.SetPos(pos.x, pos.y, pos.z);
		}
		else light.SetPos(0, 0, 0);

		App->renderer3D->SetLightToRender(&light);
		
	}
		
}

void ComponentLight::OnEditor()
{
}

uint ComponentLight::PrepareToSave() const
{
	return uint();
}

void ComponentLight::Save(char *& cursor) const
{
}
