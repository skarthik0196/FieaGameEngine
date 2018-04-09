#include "pch.h"
#include "EventQueue.h"
#include <algorithm>

namespace FieaGameEngine
{
	void FieaGameEngine::EventQueue::Enqueue(std::shared_ptr<EventPublisher> event, GameTime& gTime, const std::chrono::seconds& delay)
	{
		event->SetTime(gTime.CurrentTime(), delay);
		Queue.push_back(event);
	}

	void FieaGameEngine::EventQueue::Update(GameTime& gTime)
	{
		if (Queue.size() > 0)
		{
			std::chrono::high_resolution_clock::time_point currentTime = gTime.CurrentTime();
			auto partitionIterator = std::partition(Queue.begin(), Queue.end(), [&currentTime](std::shared_ptr<EventPublisher> event) {return event->isExpired(currentTime); });

			for (auto It = Queue.begin(); It != partitionIterator; ++It)
			{
				(*It)->Deliver();
			}
			Queue.erase(Queue.begin(), partitionIterator);
		}
	}

	void FieaGameEngine::EventQueue::Send(std::shared_ptr<EventPublisher> event)
	{
		auto It = std::find(Queue.begin(), Queue.end(), event);
		if (It != Queue.end())
		{
			(*It)->Deliver();
			Queue.erase(It);
		}
	}

	void FieaGameEngine::EventQueue::Clear()
	{
		Queue.clear();
	}

	bool FieaGameEngine::EventQueue::IsEmpty() const
	{
		return Queue.empty();
	}

	uint32_t FieaGameEngine::EventQueue::Length() const
	{
		return static_cast<uint32_t>(Queue.size());
	}
}