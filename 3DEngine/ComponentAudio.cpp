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
	if (ImGui::Button("Play Effect"))
		emitter->PlayEvent("Zoo");
	if (ImGui::Button("Play Overworld Music"))
		emitter->PlayEvent("Play_Overworld_Music");
	if (ImGui::Button("Play Battle Music"))
		emitter->PlayEvent("Play_Battle_Music");
	if (ImGui::Button("Stop Music"))
		emitter->PlayEvent("Stop_Music");
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
