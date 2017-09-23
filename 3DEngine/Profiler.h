#ifndef __PROFILER_H__
#define __PROFILER__

#include <vector>
#include "Timer.h"

class Profiler
{
public: 
	Profiler() {};
	~Profiler() {};

	float last_check_ms;
	Timer timer;

	void StartComprobation()
	{
		timer.Start();
	}

	float EndComprobation()
	{
		last_check_ms = timer.ReadMS();
		return last_check_ms;
	}
	
};



#endif