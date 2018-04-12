#pragma once
#include "RTTI.h"
#include <chrono>
#include <memory>
#include <vector>

namespace FieaGameEngine
{
	class EventSubscriber;

	/// <summary>
	/// Event Publisher class that inherits from RTTI
	/// </summary>
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI)
	public:
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~EventPublisher();

		/// <summary>
		/// Set Delay and Enqueued time for the Event
		/// </summary>
		/// <param name="enqueuedTime">High Resolution Time Point</param>
		/// <param name="delay">Delay as std::chrono::duration</param>
		void SetTime(const std::chrono::high_resolution_clock::time_point& enqueuedTime, const std::chrono::milliseconds& delay = std::chrono::seconds(0));

		/// <summary>
		/// Get Time Enqueued
		/// </summary>
		/// <returns>Time Enqueued as a high resolution Time Point</returns>
		const std::chrono::high_resolution_clock::time_point& GetTimeEnqueued() const;

		/// <summary>
		/// Delay before event is fired
		/// </summary>
		/// <returns>Delay as Duration in milliseconds</returns>
		const std::chrono::milliseconds& GetDelay() const;

		/// <summary>
		/// Returns a bool stating whether the Event has expired or not, CurrentTime > EnqueuedTime + Delay
		/// </summary>
		/// <param name="currentTime">High Resolution time point</param>
		/// <returns>A boolean</returns>
		bool isExpired(const std::chrono::high_resolution_clock::time_point& currentTime) const;

		/// <summary>
		/// Deliver the Event to all subscribers
		/// </summary>
		void Deliver();

	protected:
		explicit EventPublisher(std::vector<EventSubscriber*>* subscriberList);

		void SetSubscriberList(std::vector<EventSubscriber*>* subscriberList);

	private:
		std::chrono::milliseconds Delay{ 0 };
		std::chrono::high_resolution_clock::time_point EnqueuedTime;
		std::vector<EventSubscriber*>* SubscriberList;
	};
}

