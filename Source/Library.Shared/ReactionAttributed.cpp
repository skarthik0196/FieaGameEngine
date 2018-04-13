#include "pch.h"
#include "ReactionAttributed.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "World.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ReactionAttributed)

	ReactionAttributed::ReactionAttributed() : Reaction(TypeIdInstance())
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}


	ReactionAttributed::ReactionAttributed(const std::string& name) : Reaction(TypeIdInstance(), name)
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(const ReactionAttributed& rhs) : Reaction(rhs)
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(ReactionAttributed&& rhs) : Reaction(std::move(rhs))
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::~ReactionAttributed()
	{
		Event<EventMessageAttributed>::Unsubscribe(*this);
	}

	Vector<Signature> ReactionAttributed::GetSignature()
	{
		Vector<Signature> signature = Reaction::GetSignature();
		signature.PushBack(Signature("SubType", Datum::DatumType::String, offsetof(ReactionAttributed, SubType), 1));

		return signature;
	}

	void ReactionAttributed::SetSubType(const std::string & subType)
	{
		SubType = subType;
	}

	const std::string & ReactionAttributed::GetSubType()
	{
		return SubType;
	}

	void ReactionAttributed::Notify(EventPublisher* event)
	{
		assert(event->Is(Event<EventMessageAttributed>::TypeIdClass()));
		EventMessageAttributed& message = static_cast<Event<EventMessageAttributed>*>(event)->Message();

		if (message.GetSubType() == SubType)
		{
			for (std::string& value : message.GetAuxillaryAttributes())
			{
				Datum& datum = message[value];
				if (datum.GetType() == Datum::DatumType::Table)
				{
					Datum& tableDatum = AppendAuxillaryAttribute(value);
					tableDatum.SetType(Datum::DatumType::Table);
					for (uint32_t i = 0; i < datum.Length(); ++i)
					{
						Scope *newScope = new Scope(*datum.Get<Scope*>(i));
						tableDatum.PushBack(newScope);
					}
				}
				else
				{
					AppendAuxillaryAttribute(value) = (*this)[value];
				}
			}

			ActionList::Update((message.GetWorld())->GetWorldState());
		}
	}
}
