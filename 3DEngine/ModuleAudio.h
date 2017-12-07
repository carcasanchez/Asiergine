#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"



class ModuleAudio : public Module
{
public:

	ModuleAudio( bool start_enabled = true);
	~ModuleAudio();

	bool Init(const JSON_Object* config_data);
	bool CleanUp();



	
	int audio_driver = 0;
	int audio_device = 0;

private:
	
	int volume = 50;

};

#endif // __ModuleAudio_H__