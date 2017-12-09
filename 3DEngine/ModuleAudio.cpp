#include "Globals.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "ModuleAudio.h"
#include "wwished.h"



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

	unsigned long bank1_id = Wwished::Utility::LoadBank("Main.bnk");


	//Wwished::Utility::CreateEmitter(23, "emitter1", 0, 5, 0);

	return ret;
}

update_status ModuleAudio::Update(float real_dt, float game_dt)
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
