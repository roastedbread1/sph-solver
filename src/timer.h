#pragma once 

#include <chrono>
#include <string>
#include <iostream>


namespace TIMER
{
	struct  Timer
	{
		std::string name;
		std::chrono::time_point<std::chrono::high_resolution_clock> start;
	};

	void startTimer(Timer* timer, const std::string& name);
	void endTimer(Timer* timer);

}

