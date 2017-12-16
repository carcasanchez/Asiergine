#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "wwished.h"

class ComponentAudio;
class ComponentReverb;
class GameObject;

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


	Wwished::SoundEmitter* CreateSoundEmitter(const char* name, float3 position);
	void DeleteSoundEmitter(Wwished::SoundEmitter*);
	void ChangeState(const char* group, const char* new_state);

	bool SetListener(ComponentAudio* c);
	void CheckIfListenerIsDeleted(ComponentAudio* c);

	void LoadBank(const char* name);
	void UnLoadBank(uint bank_index);
	void UnloadAllBanks();

	void AddReverbArea(ComponentReverb*);
	void SubstracReverbArea(ComponentReverb*);

	void StopSounds();
	float GetVolume() const { return volume; }
	void ChangeVolume(float volume);
	void ChangeObjVolume(float volume, unsigned long obj_id);
	void ChangeObjPitch(float pitch, unsigned long obj_id);

	bool CheckReverb(GameObject*);

	long GetListenerID() const;

	std::vector <std::string> loaded_banks;

private:

	std::vector <ComponentReverb*> reverb_areas;

	ComponentAudio* current_listener = nullptr;
		
	std::list <Wwished::SoundEmitter*> sound_emitters;
	unsigned long last_go_id = 1;

	float volume = 50.0;

};

#endif // __ModuleAudio_H__