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

	//Scene serialization------------------------
	uint PrepareToSave() const;
	void Save(char* &cursor) const;

private:
	AUDIO_TYPE  audio_type = FX;
	Wwished::SoundEmitter* emitter = nullptr;
	AABB box = AABB::AABB(float3(-1.0f, -1.0f, -1.0f), float3(1.0f, 1.0f, 1.0f));


	float music_change_time = 0.0f;

	Timer music_timer;
};



#endif
