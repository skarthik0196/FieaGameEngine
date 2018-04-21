#include "pch.h"
#include "EventQueue.h"
#include <algorithm>
#include<future>

namespace FieaGameEngine
{
	void FieaGameEngine::EventQueue::Enqueue(std::shared_ptr<EventPublisher> event, GameTime& gTime, const std::chrono::milliseconds& delay)
	{
		std::lock_guard<std::mutex> lock(Mutex);
		event->SetTime(gTime.CurrentTime(), delay);
		NonExpiredQueue.push_back(event);
		//Queue.push_back(event);
	}

	void FieaGameEngine::EventQueue::Update(GameTime& gTime)
	{
		if (NonExpiredQueue.size() > 0)
		{
			std::vector<std::future<void>> futures;
			//std::lock_guard<std::mutex> lock(Mutex);
			std::chrono::high_resolution_clock::time_point currentTime = gTime.CurrentTime();
			auto partitionIterator = std::partition(NonExpiredQueue.begin(), NonExpiredQueue.end(), [&currentTime](std::shared_ptr<EventPublisher> event) {return event->isExpired(currentTime); });
			std::move(NonExpiredQueue.begin(), partitionIterator, std::back_inserter(Queue));
			NonExpiredQueue.erase(NonExpiredQueue.begin(), partitionIterator);

			for (auto It = Queue.begin(); It != Queue.end(); ++It)
			{
				futures.emplace_back(std::async(std::launch::async, [this, It]() {(*It)->Deliver(); }));
			}

			for (auto& f : futures)
			{
				f.get();
			}

			Queue.erase(Queue.begin(), Queue.end());
		}
	}

	void FieaGameEngine::EventQueue::Send(std::shared_ptr<EventPublisher> event)
	{
		std::lock_guard < std::mutex> lock(Mutex);
		auto It = std::find(NonExpiredQueue.begin(), NonExpiredQueue.end(), event);
		if (It != NonExpiredQueue.end())
		{
			(*It)->Deliver();
			NonExpiredQueue.erase(It);
		}
	}

	void FieaGameEngine::EventQueue::Clear()
	{
		NonExpiredQueue.clear();
	}

	bool FieaGameEngine::EventQueue::IsEmpty() const
	{
		return NonExpiredQueue.empty();
	}

	uint32_t FieaGameEngine::EventQueue::Length() const
	{
		return static_cast<uint32_t>(NonExpiredQueue.size());
	}

	std::mutex& EventQueue::GetMutex()
	{
		return Mutex;
	}
}