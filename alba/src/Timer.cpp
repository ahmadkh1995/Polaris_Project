#include "Timer.hpp"

#ifdef _WIN32
#include <Windows.h>
#elif unix
#endif

#include <iostream>

/**
* @brief Default constructor of TechUserProxy
*/
Timer::Timer(const double& rate_){

	// Set the Timer rate
	this->setRate(rate_);

}

/**
* @brief Default destroyer of TechUserProxy
*/
Timer::~Timer(){}


/**
* @brief Set function of the timer rate variable
*/
void Timer::setRate(const double& r){

	this->rate = r;

}

/**
* @brief Get function of the current time instant
*/
double Timer::getCurTime(){

	double time;

#ifdef _WIN32
	LARGE_INTEGER time_i;
	QueryPerformanceCounter(&time_i);
	time = (double)time_i.QuadPart;
#elif unix

#endif


	return time;

}

/**
* @brief Sleep function
* @param sec the time to sleep in seconds
*/
void Timer::timeSleep(const double& sec){

#ifdef _WIN32
	Sleep((DWORD)1000 * sec); // Sleep(ms)
#elif unix

#endif

}

/**
* @brief Get function of the elapsed time between two instants
* @param tic starting instant
* @param toc final instant
* @return the elapsed time
*/
double Timer::elapsedTime(const double& tic, const double& toc){

	double dt;

#ifdef _WIN32
	LARGE_INTEGER rate;
	QueryPerformanceFrequency(&rate);
	dt = (toc - tic)/(double)rate.QuadPart;
#elif unix

#endif

	return dt;
}
