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
	DeleteAllEvents();
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

	App->audio->CheckReverb(game_object);

	if (App->IsAppRunning())
	{
		if (audio_type == FX)
			ManageEvents();
		if (audio_type == MUSIC)
			ManageMusic();
	}

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

		App->renderer3D->SetSphereToDraw(math::Sphere(pos, 1.5));
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
	size += sizeof(uint) *2 ; //Type of audio & num of events

	size += sizeof(float) *2; //Volume and pitch
	
	for (int i = 0; i < events.size(); i++)
	{
		size += sizeof(uint); //size of the name of the event
		size += events[i]->name.size(); //name of the event
		size += sizeof(uint); //type of play parameter

		size += sizeof(uint); //size of state_group name
		size += events[i]->state_group.size(); //state group
		size += sizeof(uint); //size of state_1 name
		size += events[i]->state1.size(); //state 1
		size += sizeof(uint); //size of state_2 name
		size += events[i]->state2.size(); //state 2
		size += sizeof(float); //change time
	}

	return size;
}

void ComponentAudio::Save(char *& cursor) const
{
	uint audio_t = audio_type;

	//copy type of audio
	uint size_of = sizeof(uint);
	memcpy(cursor, &audio_t, size_of);
	cursor += size_of;

	//copy volume
	size_of = sizeof(float);
	memcpy(cursor, &volume, size_of);
	cursor += size_of;

	//copy pitch
	size_of = sizeof(float);
	memcpy(cursor, &pitch, size_of);
	cursor += size_of;

	//copy num of events
	uint num_of_events = events.size();
	memcpy(cursor, &num_of_events, size_of);
	cursor += size_of;
	for (int i = 0; i < num_of_events; i++)
	{
		uint name_size = events[i]->name.size();
		size_of = sizeof(uint);
		memcpy(cursor, &name_size, size_of); // copy size of name of the event
		cursor += size_of;

		size_of = events[i]->name.size();
		memcpy(cursor, events[i]->name.data(), size_of); // copy name of the event
		cursor += size_of;

		uint play_param = events[i]->play_parameter;
		size_of = sizeof(uint); //copy type of play parameter
		memcpy(cursor, &play_param, size_of);
		cursor += size_of;

		//state group
		uint state_group_size = events[i]->state_group.length();
		size_of = sizeof(uint); 
		memcpy(cursor, &state_group_size, size_of);
		cursor += size_of;
		size_of = state_group_size;
		memcpy(cursor, events[i]->state_group.c_str(), size_of);
		cursor += size_of;

		//state1
		uint state_1_size = events[i]->state1.length();
		size_of = sizeof(uint);
		memcpy(cursor, &state_1_size, size_of);
		cursor += size_of;
		size_of = state_1_size;
		memcpy(cursor, events[i]->state1.c_str(), size_of);
		cursor += size_of;

		//state2
		uint state_2_size = events[i]->state2.length();
		size_of = sizeof(uint);
		memcpy(cursor, &state_2_size, size_of);
		cursor += size_of;
		size_of = state_2_size;
		memcpy(cursor, events[i]->state2.c_str(), size_of);
		cursor += size_of;

		//change time
		size_of = sizeof(float);
		memcpy(cursor, &events[i]->change_time, size_of);
		cursor += size_of;
	}

}

void ComponentAudio::ChangeVolume(float volume)
{
	this->volume = volume;
	App->audio->ChangeObjVolume(volume, emitter->GetID());
}

void ComponentAudio::ChangePitch(float pitch)
{
	this->pitch = pitch;
	App->audio->ChangeObjPitch(pitch, emitter->GetID());
}

void ComponentAudio::ApplyReverb(float value, const char* target_bus)
{
	emitter->SetAuxiliarySends(value,target_bus, emitter->GetID());
}

AudioEvent* ComponentAudio::CreateAudioEvent(const char * name, PLAY_PARAMETER p)
{
	AudioEvent* new_event = new AudioEvent();
	new_event->name = name;
	new_event->play_parameter = p;

	events.push_back(new_event);
	return new_event;
}

void ComponentAudio::DeleteAudioEvent(uint index)
{

	emitter->StopEvent(events[index]->name.c_str());

	delete events[index];
	events.erase((events.begin() + index));
}

void ComponentAudio::DeleteAllEvents()
{
	for (int i = 0; i < events.size(); i++)
	{
		delete events[i];
	}
	events.clear();
}

void ComponentAudio::ManageEvents()
{
	
	for (int i = 0; i < events.size(); i++)
	{
		if (events[i]->play_parameter == WHEN_PRESS_E && App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			emitter->PlayEvent(events[i]->name.c_str());
		else if (events[i]->play_parameter == ON_AWAKE && events[i]->is_playing == false)
		{
			events[i]->is_playing = true;
			emitter->PlayEvent(events[i]->name.c_str());
		}
	}

}

void ComponentAudio::ManageMusic()
{
	for (int i = 0; i < events.size(); i++)
	{
		if (events[i]->is_playing == false)
		{
			events[i]->is_playing = true;
			events[i]->timer.Start();
			events[i]->current_state = &events[i]->state1;
			emitter->PlayEvent(events[i]->name.c_str());
			App->audio->ChangeState(events[i]->state_group.c_str(), events[i]->current_state->c_str());
		}
		else
		{
			if (events[i]->timer.ReadMS()/1000 > events[i]->change_time)
			{
				
				if (events[i]->current_state == &events[i]->state1)
					events[i]->current_state = &events[i]->state2;
				else if (events[i]->current_state == &events[i]->state2)
					events[i]->current_state = &events[i]->state1;
					
				App->audio->ChangeState(events[i]->state_group.c_str(), events[i]->current_state->c_str());
				events[i]->timer.Start();

			}
		}
	}
}

void ComponentAudio::ManageEventsEditor()
{

	if (ImGui::SliderFloat("Volume", &volume, 0.0f, 100.0f, "%.1f"))
	{
		ChangeVolume(volume);
	}

	if (ImGui::SliderFloat("Pitch", &pitch, -10.0f, 10.0f, "%.1f"))
	{
		ChangePitch(pitch);
	}
	
	ImGui::SameLine();
	if (ImGui::Button("Reset"))
	{
		pitch = 0.0;
		App->audio->ChangeObjPitch(pitch, emitter->GetID());
	}

	ImGui::Text("Audio Events:");
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
	if (ImGui::SliderFloat("Volume", &volume, 0.0f, 100.0f, "%.1f"))
	{
		ChangeVolume(volume);
	}

	ImGui::Text("Music Events:");
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


		char* state_group = new char[41];
		char* state_1 = new char[41];
		char* state_2= new char[41];

		std::copy(events[i]->state_group.begin(), events[i]->state_group.end(), state_group);
		state_group[events[i]->state_group.length()] = '\0';
		std::copy(events[i]->state1.begin(), events[i]->state1.end(), state_1);
		state_1[events[i]->state1.length()] = '\0';
		std::copy(events[i]->state2.begin(), events[i]->state2.end(), state_2);
		state_2[events[i]->state2.length()] = '\0';

		//Event states
		ImGui::InputText("State group", state_group, 40);
		ImGui::InputText("State 1", state_1, 40);
		ImGui::InputText("State 2", state_2, 40);
		ImGui::InputFloat("Change time", &events[i]->change_time, 0.1, 1.0, 1);
		events[i]->state_group = state_group;
		events[i]->state1 = state_1;
		events[i]->state2 = state_2;

		delete[] state_group;
		delete[] state_1;
		delete[] state_2;


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
