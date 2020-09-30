#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include "MouseEventListener.h"

class EventListener {
public:
	EventListener(const EventListener&) = delete;
	EventListener(EventListener&&) = delete;
	EventListener& operator=(const EventListener&) = delete;
	EventListener& operator=(EventListener&&) = delete;

	EventListener() = delete;
	~EventListener();

	static void Update();
};

#endif