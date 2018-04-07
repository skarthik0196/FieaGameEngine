#pragma once
//#include "RTTI.h"

namespace FieaGameEngine
{
	class EventPublisher;

	class EventSubscriber// : public RTTI
	{
		//RTTI_DECLARATIONS(EventSubscriber, RTTI)

	public:
		EventSubscriber() = default;
		EventSubscriber(const EventSubscriber& rhs) = default;
		EventSubscriber(EventSubscriber&& rhs) = default;
		virtual ~EventSubscriber() = default;

		EventSubscriber& operator=(const EventSubscriber& rhs) = default;
		EventSubscriber& operator=(EventSubscriber&& rhs) = default;

		virtual void Notify(EventPublisher* event) = 0;
	};
}

