#ifndef _COMPONENT_AUDIO_H_
#define _COMPONENT_AUDIO_H_

#include "Component.h"
#include "Globals.h"
#include "wwished.h"

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
};



#endif
