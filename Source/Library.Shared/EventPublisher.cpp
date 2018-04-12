#include "pch.h"
#include "EventPublisher.h"
#include "EventSubscriber.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventPublisher)

	EventPublisher::EventPublisher(std::vector<EventSubscriber*>* subscriberList) : SubscriberList(subscriberList)
	{
	}

	EventPublisher::~EventPublisher()
	{
	}

	void EventPublisher::SetTime(const std::chrono::high_resolution_clock::time_point& enqueuedTime, const std::chrono::milliseconds& delay)
	{
		EnqueuedTime = enqueuedTime;
		Delay = delay;
	}

	const std::chrono::high_resolution_clock::time_point& EventPublisher::GetTimeEnqueued() const
	{
		return EnqueuedTime;
	}

	const std::chrono::milliseconds& EventPublisher::GetDelay() const
	{
		return Delay;
	}

	bool EventPublisher::isExpired(const std::chrono::high_resolution_clock::time_point& currentTime) const
	{
		return ((EnqueuedTime + Delay) < currentTime);
	}

	void EventPublisher::Deliver()
	{
		for (auto& subscriber : *SubscriberList)
		{
			subscriber->Notify(this);
		}
	}

	void EventPublisher::SetSubscriberList(std::vector<EventSubscriber*>* subscriberList)
	{
		SubscriberList = subscriberList;
	}
}
