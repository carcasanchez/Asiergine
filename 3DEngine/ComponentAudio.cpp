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
	App->audio->DeleteSoundEmitter(emitter);
}

void ComponentAudio::OnEditor()
{
	if (ImGui::Button("Play Shot"))
		emitter->PlayEvent("Shot");
	if (ImGui::Button("Play Train"))
		emitter->PlayEvent("Train");
	if (ImGui::Button("Play Music"))
		emitter->PlayEvent("Play_Music");
	if (ImGui::Button("Change to Battle Music"))
		emitter->PlayEvent("Transition_To_Battle");
	if (ImGui::Button("Change to Overworld Music"))
		emitter->PlayEvent("Transition_To_Overworld");
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
