#pragma once
#include "EventPublisher.h"
#include "GameTime.h"
#include <deque>
#include<memory>

namespace FieaGameEngine
{
	class EventQueue
	{
	public:
		EventQueue() = default;
		EventQueue(const EventQueue& rhs) = default;
		EventQueue(EventQueue&& rhs) = default;
		~EventQueue() =  default;

		EventQueue& operator=(const EventQueue& rhs) = default;
		EventQueue& operator=(EventQueue&& rhs) = default;

		void Enqueue(std::shared_ptr<EventPublisher> event, GameTime& gTime, const std::chrono::milliseconds& delay);

		void Update(GameTime& gTime);

		void Send(std::shared_ptr<EventPublisher> event);

		void Clear();

		bool IsEmpty() const;

		uint32_t Length() const;

	private:
		std::deque<std::shared_ptr<EventPublisher>> Queue;
	};
}

