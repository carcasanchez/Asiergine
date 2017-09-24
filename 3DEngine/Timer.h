#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "SDL\include\SDL.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	double ReadMS();
	Uint64 ReadTicks();


private:
	Uint64	started_at;
	static Uint64 frequency;
};

#endif //__TIMER_H__