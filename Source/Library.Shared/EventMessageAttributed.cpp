#include "pch.h"
#include "EventMessageAttributed.h"
#include "Event.h"


namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventMessageAttributed)
	EventMessageAttributed::EventMessageAttributed() : Attributed(TypeIdInstance())
	{
	}

	void EventMessageAttributed::SetSubType(const std::string& subType)
	{
		SubType = subType;
	}

	void EventMessageAttributed::SetWorld(World& worldLink)
	{
		WorldPointer = &worldLink;
	}

	const std::string & EventMessageAttributed::GetSubType() const
	{
		return SubType;
	}

	World * EventMessageAttributed::GetWorld() const
	{
		return WorldPointer;
	}
	Vector<Signature> EventMessageAttributed::GetSignature()
	{
		Vector<Signature> signature;
		signature.PushBack(Signature("SubType", Datum::DatumType::String, offsetof(EventMessageAttributed, SubType), 1));
		signature.PushBack(Signature("WorldPointer", Datum::DatumType::RTTIPointer, offsetof(EventMessageAttributed, WorldPointer), 1));

		return signature;
	}
}