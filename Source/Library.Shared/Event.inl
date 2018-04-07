#pragma once
#include "Event.h"
#include "EventSubscriber.h"

namespace FieaGameEngine
{
	template<typename T>
	RTTI_DEFINITIONS(Event<T>)

	template<typename T>
	Event<T>::Event(T& payload) : EventPublisher(&Subscribers), Payload(payload)
	{

	}

	template<typename T>
	Event<T>::Event(const Event& rhs) : EventPublisher(rhs), Payload(rhs.Payload), Subscribers(rhs.Subscribers)
	{
		SetSubscriberList(Subscribers);
	}

	template<typename T>
	Event<T>::Event(Event&& rhs) : EventPublisher(std:move(rhs)), Payload(std::move(rhs.Payload)), Subscribers(std::move(rhs.Subscribers))
	{
		SetSubscriberList(Subscribers);
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
		Subscribers.push_back(&subscriber);
	}

	template<typename T>
	void Event<T>::Unsubscribe(EventSubscriber& subscriber)
	{
		Subscribers.remove(&subscriber);
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
	T& Event<T>::Message()
	{
		return Payload;
	}

	template<typename T>
	std::vector<EventSubscriber*> Event<T>::Subscribers;
}