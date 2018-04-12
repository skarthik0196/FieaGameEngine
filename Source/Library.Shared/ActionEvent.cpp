#include "pch.h"
#include "ActionEvent.h"
#include "EventMessageAttributed.h"
#include "Event.h"
#include "World.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionEvent)
	ActionEvent::ActionEvent() : Action(TypeIdInstance())
	{
	}

	ActionEvent::ActionEvent(const std::string& name) : Action(TypeIdInstance(), name)
	{
	}

	void ActionEvent::Update(WorldState& worldState)
	{
		EventMessageAttributed message;
		message.SetWorld(*worldState.CurrentWorld);
		message.SetSubType(SubType);

		for (auto& value : GetAuxillaryAttributes())
		{
			message.AppendAuxillaryAttribute(value) = message[value];
		}

		std::shared_ptr<Event<EventMessageAttributed>> event = std::make_shared<Event<EventMessageAttributed>>(message);
		(worldState.CurrentWorld)->GetEventQueue().Enqueue(event, worldState.GetGameTime(), std::chrono::milliseconds(Delay));
	}

	Vector<Signature> ActionEvent::GetSignature()
	{
		Vector<Signature> signature = Action::GetSignature();
		signature.PushBack(Signature("Delay", Datum::DatumType::Integer, offsetof(ActionEvent, Delay), 1));
		signature.PushBack(Signature("SubType", Datum::DatumType::String, offsetof(ActionEvent, SubType), 1));

		return signature;
	}

	void ActionEvent::SetSubType(const std::string& subType)
	{
		SubType = subType;
	}

	const std::string& ActionEvent::GetSubType()
	{
		return SubType;
	}

	void ActionEvent::SetDelay(const std::chrono::milliseconds& delay)
	{
		Delay = static_cast<int32_t>(delay.count());
	}

	const std::chrono::milliseconds ActionEvent::GetDelay()
	{
		return std::chrono::milliseconds(Delay);
	}
}