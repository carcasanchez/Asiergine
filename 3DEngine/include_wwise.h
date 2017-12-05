#ifndef _INCLUDE_WWISE_H_
#define _INCLUDE_WWISE_H_

#include <AK/SoundEngine/Common/AkSoundEngine.h>	   
#include <AK/MusicEngine/Common/AkMusicEngine.h>	   
#include <AK/SoundEngine/Common/AkMemoryMgr.h>         
#include <AK/SoundEngine/Common/AkModule.h>          
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>   
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/Tools/Common/AkPlatformFuncs.h> 

/*	Uncomenting this define will set the library to "RELEASE" mode. Some chunks of code will be skipped
	to increase performance, but debug will not be possible.	*/
//#define AK_OPTIMIZED

#ifdef AK_OPTIMIZED
	
	#pragma comment( lib, "3DEngine/Audiokinetic/Release/lib/AkSoundEngine.lib")
	#pragma comment( lib, "3DEngine/Audiokinetic/Release/lib/AkMusicEngine.lib")
	#pragma comment( lib, "3DEngine/Audiokinetic/Release/lib/AkMemoryMgr.lib")
	#pragma comment( lib, "3DEngine/Audiokinetic/Release/lib/AkStreamMgr.lib")

#else

	#include <AK/Comm/AkCommunication.h>
	#pragma comment( lib, "3DEngine/Audiokinetic/Debug/lib/CommunicationCentral.lib")
	#pragma comment( lib, "3DEngine/Audiokinetic/ws2_32.lib")
	#pragma comment( lib, "3DEngine/Audiokinetic/Debug/lib/AkSoundEngine.lib")
	#pragma comment( lib, "3DEngine/Audiokinetic/Debug/lib/AkMusicEngine.lib")
	#pragma comment( lib, "3DEngine/Audiokinetic/Debug/lib/AkMemoryMgr.lib")
	#pragma comment( lib, "3DEngine/Audiokinetic/Debug/lib/AkStreamMgr.lib")

#endif

#pragma comment( lib, "3DEngine/Audiokinetic/dinput8.lib") 
#pragma comment( lib, "3DEngine/Audiokinetic/dsound.lib")  
#pragma comment( lib, "3DEngine/Audiokinetic/dxguid.lib")

/*	Memory allocation functions. These are declared extern by AkMemoryMgr.h,
	but must be defined by the developer	*/
namespace AK
{
#ifdef WIN32
	void * AllocHook(size_t in_size);
	void FreeHook(void * in_ptr);
	void * VirtualAllocHook(void * in_pMemAddress, size_t in_size, DWORD in_dwAllocationType, DWORD in_dwProtect);
	void VirtualFreeHook(void * in_pMemAddress, size_t in_size, DWORD in_dwFreeType);
#endif
}

#endif