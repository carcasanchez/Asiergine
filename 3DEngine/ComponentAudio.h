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


	void ManageEvents();
	void ManageMusic();


	AUDIO_TYPE  audio_type = FX;
	std::string bank_name;
	Wwished::SoundEmitter* emitter = nullptr;

	std::list<std::string> events;

	AABB box = AABB::AABB(float3(-1.0f, -1.0f, -1.0f), float3(1.0f, 1.0f, 1.0f));
};



#endif
