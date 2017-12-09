#ifndef _WWISHED_H_
#define _WWISHED_H_


#include <AK/SoundEngine/Common/AkTypes.h>


namespace Wwished
{
	//Life cycle functions
	bool InitWwished(const wchar_t* base_path, const char* language);
	void ProcessAudio();
	bool CloseWwished();
	
	
	class SoundEmitter
	{
	public:

		SoundEmitter(unsigned long id, const char* n);
		~SoundEmitter();

		unsigned long GetID();
		const char* GetName();
		void SetPosition(float x, float y, float z);
		void SetOrientation(float x_front, float y_front, float z_front, float x_top, float y_top, float z_top);

	private:
		unsigned long EmitterID = 0;
		const char* name = nullptr;
		AkVector position;
		AkVector orient_top;
		AkVector orient_front;
	};


	namespace Utility
	{
		void SetLanguage(const char* language);
		unsigned long LoadBank(const char* path);
		SoundEmitter* CreateEmitter(unsigned long id, const char* name, float x, float y, float z);
	}

}

#endif
