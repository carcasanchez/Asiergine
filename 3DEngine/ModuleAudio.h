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
	update_status PostUpdate(float real_dt, float game_dt);
	bool CleanUp();
		
	
	int audio_driver = 0;
	int audio_device = 0;


	Wwished::SoundEmitter* CreateSoundEmitter(unsigned long id, const char* name, float3 position);
	void DeleteSoundEmitter(Wwished::SoundEmitter*);


private:

	Wwished::SoundEmitter* camera_listener = nullptr;
	
	std::list <Wwished::SoundEmitter*> sound_emitters;

	unsigned long listener_id = 0;

};

#endif // __ModuleAudio_H__