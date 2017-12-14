#include "ComponentAudio.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "GameObject.h"
#include "CompTransform.h"
#include "imgui\imgui.h"

ComponentAudio::ComponentAudio(GameObject* g):Component(g)
{
	type = COMPONENT_AUDIO;
	
	bank_name.reserve(51);

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

	int selected_opt = audio_type;
	if (ImGui::Combo("Audio type", &selected_opt, "FX\0Music\0Listener", 3))
	{
		if (selected_opt == 2)
		{
			audio_type = LISTENER;
			App->audio->SetListener(this);
		}
		else if (selected_opt == 0)
			audio_type = FX;
		else if (selected_opt == 1)
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
		Quat rot = transf->GetRotation();

		float3 up = rot.Transform(float3(0, 1, 0));
		float3 front = rot.Transform(float3(0, 0, 1));

		up.Normalize();
		front.Normalize();

		emitter->SetPosition(-pos.x, pos.y, pos.z, -front.x, front.y, front.z, -up.x, up.y, up.z);

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
}

void ComponentAudio::SetAudioType(AUDIO_TYPE t)
{
	audio_type = t;

	if(audio_type==LISTENER)
	{
		App->audio->SetListener(this);
	}
}

uint ComponentAudio::PrepareToSave() const
{
	uint size = 0;
	size += sizeof(uint); //Type of audio
	return size;
}

void ComponentAudio::Save(char *& cursor) const
{
	uint audio_t = audio_type;

	//copy type of audio
	uint size_of = sizeof(uint);
	memcpy(cursor, &audio_t, size_of);
	cursor += size_of;
}

void ComponentAudio::ManageEvents()
{
}

void ComponentAudio::ManageMusic()
{
}
