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

	Wwished::Utility::LoadBank("Main.bnk");
		
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

