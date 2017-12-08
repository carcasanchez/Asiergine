#ifndef _WWISHED_H_
#define _WWISHED_H_



namespace Wwished
{
	//Life cycle functions
	bool InitWwished(const char* base_path, const char* language);
	void ProcessAudio();
	bool CloseWwished();


	namespace Utility
	{
		void SetLanguage(const char* language);
	}
}

#endif
