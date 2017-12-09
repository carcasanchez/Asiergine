#include "Globals.h"
#include "Application.h"
#include ".\mmgr\mmgr.h"
#include "ModuleAudio.h"



ModuleAudio::ModuleAudio( bool start_enabled) : Module(start_enabled)
{
	name = "audio";
}

// Destructor
ModuleAudio::~ModuleAudio()
{
	//delete emitter1;
}

// Called before render is available
bool ModuleAudio::Init(const JSON_Object* config_data)
{
	LOG("Loading Wwished library");


	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;	
	std::wstring base_path = converter.from_bytes((App->fs->GetAssetDirectory() + "Soundbanks/").c_str());

	bool ret = Wwished::InitWwished(base_path.c_str(), "English(US)");

	unsigned long bank1_id = Wwished::Utility::LoadBank("Main.bnk");
		
	return ret;
}

bool ModuleAudio::Start()
{
	listener = Wwished::Utility::CreateEmitter(0, "listener", 0, 0, 0);
	listener_id = listener->GetID();
	Wwished::SetDefaultListeners(&listener_id);
	listener->SetPosition(0, 0, 30);

	emitter1 = Wwished::Utility::CreateEmitter(2, "emitter1", 5, 0, 0, &listener_id);
	//emitter1->SetListener(&listener_id);

	return true;
}

update_status ModuleAudio::Update(float real_dt, float game_dt)
{
	

	if(App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		emitter1->PlayEvent("Fire_Shotgun_Player");

	

	Wwished::ProcessAudio();

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	
	LOG("Unloading Wwished library");
	return Wwished::CloseWwished();
}
