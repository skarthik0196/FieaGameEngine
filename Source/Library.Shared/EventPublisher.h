#pragma once
#include "RTTI.h"
#include <chrono>
#include <memory>
#include <vector>

namespace FieaGameEngine
{
	class EventSubscriber;

	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI)
	public:
		explicit EventPublisher(std::vector<EventSubscriber*>* subscriberList);

		virtual ~EventPublisher();

		void SetTime(const std::chrono::high_resolution_clock::time_point& enqueuedTime, const std::chrono::seconds& delay = std::chrono::seconds(0));

		const std::chrono::high_resolution_clock::time_point& GetTimeEnqueued() const;
		const std::chrono::seconds& GetDelay() const;

		bool isExpired(const std::chrono::high_resolution_clock::time_point& currentTime) const;

		void Deliver();

	protected:
		void SetSubscriberList(std::vector<EventSubscriber*>* subscriberList);

	private:
		std::chrono::seconds Delay{ 0 };
		std::chrono::high_resolution_clock::time_point EnqueuedTime;
		std::vector<EventSubscriber*>* SubscriberList;
	};
}

