#pragma once
#include "EventPublisher.h"

namespace FieaGameEngine
{
	template<typename T>
	class Event : public EventPublisher
	{
		RTTI_DECLARATIONS(Event, EventPublisher)

	public:
		explicit Event(T& payload);
		Event(const Event& rhs);
		Event(Event&& rhs);
		~Event();

		Event& operator=(const Event& rhs);
		Event& operator=(Event&& rhs);
		
		static void Subscribe(EventSubscriber& subscriber);
		static void Unsubscribe(EventSubscriber& subscriber);
		static void UnsubscribeAll();

		static void ReserveSpace(const uint32_t& size);
		static void ShrinkToFitSubscriberList();

		T& Message();

	private:
		T Payload;

		static std::vector<EventSubscriber*> Subscribers;
	};
}
#include "Event.inl"