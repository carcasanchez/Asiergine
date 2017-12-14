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


struct AudioEvent
{
	bool is_playing = false;
	std::string name;
	PLAY_PARAMETER play_parameter;
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

	void CreateAudioEvent(const char* name, PLAY_PARAMETER);
	void DeleteAudioEvent(uint index);


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

	AABB box = AABB::AABB(float3(-1.0f, -1.0f, -1.0f), float3(1.0f, 1.0f, 1.0f));
};



#endif
