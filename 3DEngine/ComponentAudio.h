#ifndef _COMPONENT_AUDIO_H_
#define _COMPONENT_AUDIO_H_

#include "Component.h"
#include "Globals.h"
#include "wwished.h"
#include "Timer.h"

enum AUDIO_TYPE
{
	FX,
	MUSIC
};

class ComponentAudio : public Component
{
public:
	ComponentAudio(GameObject*);
	~ComponentAudio();

	void OnEditor();

	void Update(float real_dt, float game_dt);
	AABB GetBox() { return box; }

private:
	Wwished::SoundEmitter* emitter = nullptr;
	AABB box = AABB::AABB(float3(-1.0f, -1.0f, -1.0f), float3(1.0f, 1.0f, 1.0f));
	AUDIO_TYPE  audio_type = FX;

	std::string event_name;
	std::string state1_name;
	std::string state2_name;
	std::string* current_music_state = nullptr;

	float music_change_time = 0.0f;

	Timer music_timer;

};



#endif
