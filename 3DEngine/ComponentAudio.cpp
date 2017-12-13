#include "ComponentAudio.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "GameObject.h"
#include "CompTransform.h"
#include "imgui\imgui.h"

ComponentAudio::ComponentAudio(GameObject* g):Component(g)
{
	type = COMPONENT_AUDIO;
	

	CompTransform* transf = (CompTransform*)g->GetComponentByType(COMPONENT_TRANSFORM);
	if (transf)
	{
		emitter = App->audio->CreateSoundEmitter(g->GetName(), transf->GetTranslation());
	}
}


ComponentAudio::~ComponentAudio()
{
	App->audio->CheckIfListenerIsDeleted(this);
	App->audio->DeleteSoundEmitter(emitter);
}

void ComponentAudio::OnEditor()
{
	
	if (ImGui::Combo("Audio type", &selected_option, "FX\0Music\0Listener", 3))
	{
		if (selected_option == 2)
		{
			audio_type = LISTENER;
			App->audio->SetListener(this);
		}
		else if (selected_option == 0)
			audio_type = FX;
		else if (selected_option == 1)
			audio_type = MUSIC;
	}

	if (audio_type == FX && ImGui::Button("Play!"))
	{
		emitter->PlayEvent("Shot");
	}

	/*ImGui::InputText("Event Name", (char*)event_name.c_str(), 40);

	if (ImGui::Button("Play!"))
	{
		emitter->PlayEvent(event_name.c_str());
		music_timer.Start();
		current_music_state = &state1_name;
	}

	if (audio_type == MUSIC)
	{
		if (ImGui::Button("Stop Music"))
		{
			emitter->PlayEvent("Stop_Music");
		}

		ImGui::InputText("Music State 1", (char*)state1_name.c_str(), 40);
		ImGui::InputText("Music State 2", (char*)state2_name.c_str(), 40);
		ImGui::InputFloat("Change Time", &music_change_time, 0.1, 1.0, 1);
	}

	*/
	
}

void ComponentAudio::Update(float real_dt, float game_dt)
{
	/*if(audio_type == MUSIC)
		if (current_music_state && music_timer.ReadMS()/1000 > music_change_time)
		{
			music_timer.Start();
			if (current_music_state == &state1_name)
				current_music_state = &state2_name;
			else if (current_music_state == &state2_name)
				current_music_state = &state1_name;
			App->audio->ChangeState("music1", current_music_state->c_str());
		}
*/

	CompTransform* transf = (CompTransform*)game_object->GetComponentByType(COMPONENT_TRANSFORM);
	if (transf)
	{
		float3 pos = transf->GetTranslation();
		emitter->SetPosition(pos.z, pos.y, pos.x);

		box.minPoint = transf->GetTranslation() - float3(1, 1, 1);
		box.maxPoint = transf->GetTranslation() + float3(1, 1, 1);
	}

	if (App->scene->debug_boxes)
	{
		App->renderer3D->SetBoxToDraw(box);
	}

}

void ComponentAudio::ResetComponent()
{
	audio_type = FX;
	selected_option = 0;
}
