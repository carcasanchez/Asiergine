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
	for (int i = 0; i < events.size(); i++)
	{
		delete events[i];
	}

	events.clear();
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

	if (audio_type == FX)
		ManageEventsEditor();
	else if (audio_type == MUSIC)
		ManageMusicEditor();
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

void ComponentAudio::CreateAudioEvent(const char * name, PLAY_PARAMETER p)
{
	AudioEvent* new_event = new AudioEvent();
	new_event->name = name;
	new_event->play_parameter = p;

	events.push_back(new_event);
}

void ComponentAudio::DeleteAudioEvent(uint index)
{

	emitter->StopEvent(events[index]->name.c_str());

	delete events[index];
	events.erase((events.begin() + index));
}

void ComponentAudio::ManageEvents()
{
}

void ComponentAudio::ManageMusic()
{
}

void ComponentAudio::ManageEventsEditor()
{
	ImGui::Text("AudioEvents:");
	for (int i = 0; i < events.size();)
	{
		ImGui::Separator();

		ImGui::Text("%s", events[i]->name.c_str());
	
		ImGui::PushID(i);
		if (ImGui::Button("Play"))
		{
			emitter->PlayEvent(events[i]->name.c_str());
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			emitter->StopEvent(events[i]->name.c_str());
		}
		ImGui::SameLine();
		int selected_opt = (int)events[i]->play_parameter;
		if (ImGui::Combo("", &selected_opt, "When pressing E\0On Awake", 2))
		{
			if (selected_opt == 0)
				events[i]->play_parameter = WHEN_PRESS_E;
			else if (selected_opt == 1)
				events[i]->play_parameter = ON_AWAKE;
		}

		if (ImGui::Button("Delete Event"))
		{
			DeleteAudioEvent(i);
		}
		else  i++;
		ImGui::PopID();

	}

	ImGui::Separator();
	static char new_event_name[51];
	ImGui::InputText("", new_event_name, 50);
	ImGui::SameLine();
	if (ImGui::Button("New Event"))
	{
		CreateAudioEvent(new_event_name, WHEN_PRESS_E);
	}
}

void ComponentAudio::ManageMusicEditor()
{
}
