#include "pch.h"
#include "EventPublisher.h"
#include "EventSubscriber.h"


namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventPublisher)

	EventPublisher::EventPublisher(std::vector<EventSubscriber*>* subscriberList, std::mutex& derivedMutex) : SubscriberList(subscriberList), DerivedMutex(&derivedMutex)
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
		std::vector<std::future<void>> futures;

		{
			std::unique_lock<std::mutex> lock(*DerivedMutex);

			for (auto& subscriber : *SubscriberList)
			{
				futures.emplace_back(std::async(std::launch::async, [this, subscriber]() {subscriber->Notify(this); }));;
				//subscriber->Notify(this);
			}
		}
		for (auto& f : futures)
		{
			
			f.get();
		}
	}

	std::mutex& EventPublisher::GetMutex()
	{
		return *DerivedMutex;
	}

	void EventPublisher::SetSubscriberList(std::vector<EventSubscriber*>* subscriberList)
	{
		SubscriberList = subscriberList;
	}
}
