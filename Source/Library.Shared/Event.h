#pragma once
#include "EventPublisher.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Templated Event Class that derives from EventPublisher
	/// </summary>
	template<typename T>
	class Event : public EventPublisher
	{
		RTTI_DECLARATIONS(Event, EventPublisher)

	public:
		/// <summary>
		/// Explicit Constructor that takes in a reference to the payload
		/// </summary>
		explicit Event(T& payload);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Reference to rhs</param>
		Event(const Event& rhs);

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs">R value reference to the rhs</param>
		Event(Event&& rhs);

		/// <summary>
		/// Destructor
		/// </summary>
		~Event();

		/// <summary>
		/// Copy Assignment operator
		/// </summary>
		/// <param name="rhs">Reference to rhs</param>
		/// <returns>Reference to the coped event</returns>
		Event& operator=(const Event& rhs);

		/// <summary>
		/// Move Assignment operator
		/// </summary>
		/// <param name="rhs">R value refernce to rhs</param>
		/// <returns>Reference to the copied event</returns>
		Event& operator=(Event&& rhs);
		
		/// <summary>
		/// Subscribe to this event
		/// </summary>
		/// <param name="subscriber">Reference to event subscriber</param>
		static void Subscribe(EventSubscriber& subscriber);

		/// <summary>
		/// Unsubscribe from this event
		/// </summary>
		/// <param name="subscriber">Reference to the event subscriber</param>
		static void Unsubscribe(EventSubscriber& subscriber);

		/// <summary>
		/// Unsubscribe all from this event
		/// </summary>
		static void UnsubscribeAll();

		/// <summary>
		/// Reserve Space in the static vector
		/// </summary>
		/// <param name="size"></param>
		static void ReserveSpace(const uint32_t& size);

		/// <summary>
		/// Shrink Subscriber list to fir
		/// </summary>
		static void ShrinkToFitSubscriberList();

		/// <summary>
		/// Get the message asssociated with this event
		/// </summary>
		/// <returns>A T&</returns>
		T& Message();

	private:
		T Payload;

		static std::vector<EventSubscriber*> Subscribers;
	};
}
#include "Event.inl"