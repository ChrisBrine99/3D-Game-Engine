#include "Timer.h"

Timer::Timer() {
	prevTicks = 0;
	curTicks = 0;
}

Timer::~Timer() {
}

void Timer::Start() {
	prevTicks = SDL_GetTicks();
	curTicks = SDL_GetTicks();
}

void Timer::UpdateFrameTicks() {
	prevTicks = curTicks;
	curTicks = SDL_GetTicks();
}

unsigned int Timer::GetSleepTime(const unsigned int FPS) {
	unsigned int milliPerFrame = 1000 / FPS;
	if (milliPerFrame == 0) { // No wait necessary; return 0
		return 0;
	}
	// Get how long the engine needs to wait in-between frames; return lowest between milliPerFrame and sleepTime
	unsigned int sleepTime = milliPerFrame - SDL_GetTicks();
	if (sleepTime > milliPerFrame) {
		return milliPerFrame;
	} else {
		return sleepTime;
	}
}

float Timer::GetDeltaTime() const {
	return static_cast<float>(curTicks - prevTicks) / 1000.0f;
}

float Timer::GetCurrentTicks() const {
	return static_cast<float>(curTicks);
}
