#include "pch.h"
#include "Reaction.h"


namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Reaction)

	Vector<Signature> Reaction::GetSignature()
	{
		return ActionList::GetSignature();
	}

	Reaction::Reaction(const uint64_t& runtimeTypeID, const std::string& name) : ActionList(runtimeTypeID, name)
	{

	}

	Reaction::Reaction(const uint64_t& runtimeTypeID) : ActionList(runtimeTypeID)
	{

	}
}