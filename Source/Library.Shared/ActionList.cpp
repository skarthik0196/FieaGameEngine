#include "pch.h"
#include "ActionList.h"

namespace FieaGameEngine
{
	ActionList::ActionList()
	{
		ActionList::InitializeSignatures();
	}

	ActionList::ActionList(const ActionList& rhs) :Action(rhs)
	{

	}

	ActionList::ActionList(ActionList&& rhs) : Action(std::move(rhs))
	{
	}

	ActionList::~ActionList()
	{
	}
	ActionList& ActionList::operator=(const ActionList& rhs)
	{
		if (this != &rhs)
		{
			Action::operator=(rhs);
		}
		return *this;
	}
	ActionList& ActionList::operator=(ActionList&& rhs)
	{
		if (this != &rhs)
		{
			Action::operator=(std::move(rhs));
		}
		return *this;
	}
	Datum& ActionList::GetActions()
	{
		return (*this)["Actions"];
	}
	void ActionList::Update(WorldState& worldState)
	{
		worldState.CurrentAction = this;

		Datum& actions = GetActions();
		for (uint32_t i = 0; i < actions.Length(); ++i)
		{
			assert(actions.Get<Scope*>(i)->Is("Action"));
			static_cast<Action*>(actions.Get<Scope*>(i))->Update(worldState);
		}
	}

	void ActionList::InitializeSignatures()
	{
		AddNestedScope("Actions");
	}
}
