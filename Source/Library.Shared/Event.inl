#pragma once
#include "Event.h"
#include "EventSubscriber.h"

namespace FieaGameEngine
{
	template<typename T>
	RTTI_DEFINITIONS(Event<T>)

	template<typename T>
	Event<T>::Event(T& payload) : EventPublisher(&Subscribers, Mutex), Payload(payload)
	{

	}

	template<typename T>
	Event<T>::Event(const Event<T>& rhs) : EventPublisher(rhs), Payload(rhs.Payload)
	{
		
	}

	template<typename T>
	Event<T>::Event(Event && rhs) : EventPublisher(std:: move(rhs)), Payload(std::move(rhs.Payload))
	{

	}

	template<typename T>
	Event<T>::~Event()
	{

	}

	template<typename T>
	Event<T>& Event<T>::operator=(const Event<T>& rhs)
	{
		if (this != &rhs)
		{
			EventPublisher::operator=(rhs);
			Payload = rhs.Payload;
		}
		return *this;
	}

	template<typename T>
	Event<T>& Event<T>::operator=(Event<T>&& rhs)
	{
		if (this != &rhs)
		{
			EventPublisher::operator=(std::move(rhs));
			Payload = std::move(rhs.Payload);
		}
		return *this;
	}

	template<typename T>
	void Event<T>::Subscribe(EventSubscriber& subscriber)
	{
		std::lock_guard<std::mutex> lock(Mutex);
		Subscribers.push_back(&subscriber);
	}

	template<typename T>
	void Event<T>::Unsubscribe(EventSubscriber& subscriber)
	{
		std::lock_guard<std::mutex> lock(Mutex);
		auto It = (std::find(Subscribers.begin(), Subscribers.end(), &subscriber));
		if (It != Subscribers.end())
		{
			Subscribers.erase(It);
		}
	}

	template<typename T>
	void Event<T>::UnsubscribeAll()
	{
		Subscribers.clear();
	}

	template<typename T>
	void Event<T>::ReserveSpace(const uint32_t& size)
	{
		Subscribers.reserve(size);
	}

	template<typename T>
	inline void Event<T>::ShrinkToFitSubscriberList()
	{
		Subscribers.shrink_to_fit();
	}

	template<typename T>
	T& Event<T>::Message()
	{
		return Payload;
	}

	template<typename T>
	inline uint32_t Event<T>::SubscriberLength()
	{
		return static_cast<uint32_t>(Subscribers.size());
	}

	template<typename T>
	inline std::mutex& Event<T>::GetMutex()
	{
		return Mutex;
	}

	template<typename T>
	std::vector<EventSubscriber*> Event<T>::Subscribers;

	template<typename T>
	std::mutex Event<T>::Mutex;
}