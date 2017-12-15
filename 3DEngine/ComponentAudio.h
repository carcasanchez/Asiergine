#ifndef _COMPONENT_AUDIO_H_
#define _COMPONENT_AUDIO_H_

#include "Component.h"
#include "Globals.h"
#include "wwished.h"
#include "Timer.h"

enum AUDIO_TYPE
{
	FX,
	MUSIC,
	LISTENER
};

//Hardcode for demonstration purposes
enum PLAY_PARAMETER
{
	WHEN_PRESS_E,
	ON_AWAKE
};


class AudioEvent
{
public:
	AudioEvent()
	{
		state_group.reserve(41);
		state1.reserve(41);
		state2.reserve(41);
		state_group = "";
		state1 = "";
		state2 = "";
	};

	bool is_playing = false;
	std::string name;
	PLAY_PARAMETER play_parameter;

	std::string state_group;
	std::string state1;
	std::string state2;
	float change_time = 0.0;

	std::string* current_state = nullptr;
	Timer timer;
};


class ComponentAudio : public Component
{
public:
	ComponentAudio(GameObject*);
	~ComponentAudio();

	void OnEditor();

	void Update(float real_dt, float game_dt);
	AABB GetBox() { return box; }
	unsigned long GetEmitterID()
	{
		return emitter ?  emitter->GetID() :  0;
	}

	void ResetComponent();
	void SetAudioType(AUDIO_TYPE);

	AudioEvent* CreateAudioEvent(const char* name, PLAY_PARAMETER);
	void DeleteAudioEvent(uint index);
	void DeleteAllEvents();

	//Scene serialization------------------------
	uint PrepareToSave() const;
	void Save(char* &cursor) const;

private:
	void ManageEvents();
	void ManageMusic();
	void ManageEventsEditor();
	void ManageMusicEditor();

	AUDIO_TYPE  audio_type = FX;
	Wwished::SoundEmitter* emitter = nullptr;
	std::vector<AudioEvent*> events;

	AABB box = AABB::AABB(float3(-2.0f, -2.0f, -2.0f), float3(2.0f, 2.0f, 2.0f));
};



#endif
