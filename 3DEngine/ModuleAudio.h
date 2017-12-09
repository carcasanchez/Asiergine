#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "wwished.h"


class ModuleAudio : public Module
{
public:

	ModuleAudio( bool start_enabled = true);
	~ModuleAudio();

	bool Init(const JSON_Object* config_data);
	bool Start();
	update_status Update(float real_dt, float game_dt);
	bool CleanUp();

	
	int audio_driver = 0;
	int audio_device = 0;

private:
	
	int volume = 50;
	Wwished::SoundEmitter* emitter1 = nullptr;
	Wwished::SoundEmitter* listener = nullptr;
	unsigned long listener_id = 0;

};

#endif // __ModuleAudio_H__