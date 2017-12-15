#include "Globals.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "ModuleAudio.h"
#include "ComponentAudio.h"



ModuleAudio::ModuleAudio( bool start_enabled) : Module(start_enabled)
{
	name = "audio";
}

// Destructor
ModuleAudio::~ModuleAudio()
{}

// Called before render is available
bool ModuleAudio::Init(const JSON_Object* config_data)
{
	LOG("Loading Wwished library");

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;	
	std::wstring base_path = converter.from_bytes((App->fs->GetAssetDirectory() + "Soundbanks/").c_str());

	bool ret = Wwished::InitWwished(base_path.c_str(), "English(US)");

		
	return ret;
}

bool ModuleAudio::Start()
{
	
	return true;
}

update_status ModuleAudio::PostUpdate(float real_dt, float game_dt)
{
	
	Wwished::ProcessAudio();

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	LOG("Unloading Wwished library");
	return Wwished::CloseWwished();
}

Wwished::SoundEmitter * ModuleAudio::CreateSoundEmitter(const char * name, float3 position)
{
	Wwished::SoundEmitter* ret = Wwished::Utility::CreateEmitter(last_go_id++, name, -position.x, position.y, position.z);
	sound_emitters.push_back(ret);
	return ret;
}

void ModuleAudio::DeleteSoundEmitter(Wwished::SoundEmitter * emit)
{
	sound_emitters.remove(emit);
	delete emit;
}

void ModuleAudio::ChangeState(const char * group, const char * new_state)
{
	Wwished::Utility::ChangeState(group, new_state);
}

bool ModuleAudio::SetListener(ComponentAudio* c)
{
	bool ret = true;
	if (current_listener != nullptr)
	{
		current_listener->ResetComponent();
	}

	current_listener = c;
	Wwished::SetDefaultListener(current_listener->GetEmitterID());

	return ret;
}

void ModuleAudio::CheckIfListenerIsDeleted(ComponentAudio * c)
{
	if (current_listener == c)
	{
		current_listener = nullptr;
	}
}

void ModuleAudio::LoadBank(const char * bank_name)
{
	//Avoid double bank loading
	if (std::find(loaded_banks.begin(), loaded_banks.end(), bank_name) == loaded_banks.end())
	{
		if(Wwished::Utility::LoadBank(bank_name))
		{ 
			loaded_banks.push_back(bank_name);
			LOG("Loaded %s Wwise audio bank", bank_name)
		}
		else LOG("ERROR: Invalid bank name: %s", bank_name);
	}
	else LOG("Bank %s already loaded", bank_name);
}

void ModuleAudio::UnLoadBank(uint bank_index)
{
	if (!Wwished::Utility::UnLoadBank(loaded_banks[bank_index].c_str()))
	{
		LOG("Bank %s is not loaded in memory", name);
	}
	else
	{
		loaded_banks.erase((loaded_banks.begin() + bank_index));
	}
}

void ModuleAudio::UnloadAllBanks()
{
	for (int i = 0; i < loaded_banks.size(); i++)
	{
		Wwished::Utility::UnLoadBank(loaded_banks[i].c_str());
	}

	loaded_banks.clear();
}

void ModuleAudio::StopSounds()
{
	Wwished::Utility::StopAllSounds();
}

void ModuleAudio::ChangeVolume(float volume)
{
	Wwished::Utility::SetRTPCValue("GeneralVolume", volume);
}

void ModuleAudio::ChangeObjVolume(float volume, unsigned long obj_id)
{
	Wwished::Utility::SetRTPCValue("GeneralVolume", volume, obj_id);

}


