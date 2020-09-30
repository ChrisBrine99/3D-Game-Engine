#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

class Timer {
private:
	unsigned int prevTicks, curTicks;
public:
	Timer(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer& operator=(Timer&&) = delete;

	Timer();
	~Timer();

	void Start();
	void UpdateFrameTicks();

	unsigned int GetSleepTime(const unsigned int FPS);
	float GetDeltaTime() const;
	float GetCurrentTicks() const;
};

#endif