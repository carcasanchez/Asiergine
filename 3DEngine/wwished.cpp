#include "wwished.h"
#include <assert.h>

#include "include_wwise.h"
#include "AK/include/Win32/AkFilePackageLowLevelIOBlocking.h"

CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

//Initialize all Wwise modules. Receives the base path for soundbanks and the current language
bool Wwished::InitWwished(const wchar_t* base_path, const char* language)
{

	//Init default Wwise memory manager
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;
	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		assert(!"Could not create the memory manager.");
		return false;
	}


	//Initialize stream manager
	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);
	if (!AK::StreamMgr::Create(stmSettings))
	{
		assert(!"Could not create the Streaming Manager");
		return false;
	}

	//Initialize default IO device
	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);
	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		assert(!"Could not create the streaming device and Low-Level I/O system");
		return false;
	}


	// Create the Sound Engine using default initialization parameters
	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);
	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		return false;
	}


	// Initialize the music engine using default initialization parameters
	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);
	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		return false;
	}


#ifndef AK_OPTIMIZED
	// Initialize communications for debug purposes
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		return false;
	}
#endif 
	
	//Set base path for sound banks
	AKRESULT base_path_res = g_lowLevelIO.SetBasePath(base_path);
	if (base_path_res != AK_Success)
	{
		assert(!"Invalid base path!");
		return false;
	}

	//Set language
	Utility::SetLanguage(language);

	//Loads the Init Sound Bank
	Utility::LoadBank("Init.bnk");

	return true;
}

void Wwished::ProcessAudio()
{
	AK::SoundEngine::RenderAudio();
}

bool Wwished::CloseWwished()
{
	//Terminate comunication module (IMPORTANT: this must be the first module in being terminated)
	#ifndef AK_OPTIMIZED
		AK::Comm::Term();
	#endif 
		
	//Terminate the music engine
	AK::MusicEngine::Term();

	//Terminate the sound engine
	AK::SoundEngine::Term();

	//Terminate IO device
	g_lowLevelIO.Term();
	if (AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();

	//Terminate the Memory Manager
	AK::MemoryMgr::Term();


	return false;
}

void Wwished::Utility::SetLanguage(const char * language)
{
	AKRESULT res = AK::StreamMgr::SetCurrentLanguage((AkOSChar*)language);
	if (res == AK_Fail)
	{
		assert(!"Invalid language!");
	}
}

//Gets the bank path and returns it's ID. IMPORTANT: never call this before InitWwished()
unsigned long Wwished::Utility::LoadBank(const char * path)
{
	unsigned long bank_id;
	AKRESULT res = AK::SoundEngine::LoadBank(path, AK_DEFAULT_POOL_ID, bank_id);
	
	if (res != AK_Success)
	{
		//The .bnk file is missing
		assert(!"Could not initialize soundbank!");
	}

	return bank_id;
}
