#include "ComponentAudio.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "GameObject.h"
#include "CompTransform.h"
#include "imgui\imgui.h"

ComponentAudio::ComponentAudio(GameObject* g):Component(g)
{
	CompTransform* transf = (CompTransform*)g->GetComponentByType(COMPONENT_TRANSFORM);
	if (transf)
	{
		emitter = App->audio->CreateSoundEmitter("Shotgun", transf->GetTranslation());
	}
}


ComponentAudio::~ComponentAudio()
{
}

void ComponentAudio::OnEditor()
{
	if (ImGui::Button("Play!"))
		emitter->PlayEvent("Zoo");
}

void ComponentAudio::Update(float real_dt, float game_dt)
{
	CompTransform* transf = (CompTransform*)game_object->GetComponentByType(COMPONENT_TRANSFORM);
	if (transf)
	{
		float3 pos = transf->GetTranslation();
		emitter->SetPosition(pos.x, pos.y, pos.z);
	}
}
