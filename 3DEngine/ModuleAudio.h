#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

class ModuleAudio : public Module
{
public:

	ModuleAudio( bool start_enabled = true);
	~ModuleAudio();

	bool Init(const JSON_Object* config_data);
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	void ChangeVolume(int new_volume);

	int GetVolume()
	{
		return volume;
	}


	
	int audio_driver = 0;
	int audio_device = 0;

private:

	
	int volume = 50;
	Mix_Music*			music;
	std::vector<Mix_Chunk*> fx;
};

#endif // __ModuleAudio_H__