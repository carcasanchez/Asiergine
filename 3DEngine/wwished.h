#ifndef _WWISHED_H_
#define _WWISHED_H_


#include <AK/SoundEngine/Common/AkTypes.h>


namespace Wwished
{
	//Life cycle functions
	bool InitWwished(const wchar_t* base_path, const char* language);
	void ProcessAudio();
	bool CloseWwished();

	void SetDefaultListener(unsigned long id);
		
	class SoundEmitter
	{
	public:

		SoundEmitter(unsigned long id, const char* n);
		~SoundEmitter();

		unsigned long GetID();
		const char* GetName();
		void SetPosition(float x=0, float y = 0, float z = 0, float x_front = 1, float y_front=0, float z_front=0, float x_top=0, float y_top=1, float z_top=0);
		void SetListener(unsigned long* id);

		void PlayEvent(unsigned long id);
		void PlayEvent(const char* name);

		void StopEvent(const char* name);


	private:
		AkGameObjectID EmitterID;
		const char* name = nullptr;
		AkVector position;
		AkVector orient_top;
		AkVector orient_front;
	};


	namespace Utility
	{
		void SetLanguage(const char* language);
		bool LoadBank(const char* path);
		bool UnLoadBank(const char* path);
		SoundEmitter* CreateEmitter(unsigned long id, const char* name,  float x, float y, float z, bool is_default_listener = false);
		void ChangeState(const char* group, const char* new_state);	
	}

}

#endif
