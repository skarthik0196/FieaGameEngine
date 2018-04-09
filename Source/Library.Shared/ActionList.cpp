#include "pch.h"
#include "ActionList.h"
#include "AbstractFactory.h"
#include "ConcreteFactory.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionList)

	ActionList::ActionList() : Action(TypeIdInstance())
	{
		//ActionList::InitializeSignatures();
	}

	ActionList::ActionList(const std::string& name) : Action(TypeIdInstance(), name)
	{
		//ActionList::InitializeSignatures();
	}


	ActionList::ActionList(const uint64_t& runtimeTypeID, const std::string &name) : Action(runtimeTypeID, name)
	{

	}

	ActionList::ActionList(const uint64_t& runtimeTypeID) : Action(runtimeTypeID)
	{

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

	Action* ActionList::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Action* newAction = AbstractFactory<Action>::Create(className);
		newAction->SetName(instanceName);
		AppendAction(*newAction);

		return newAction;
	}

	void ActionList::AppendAction(Action& action)
	{
		Adopt(action, "Actions");
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

	Vector<Signature> ActionList::GetSignature()
	{
		Vector<Signature> signature = Action::GetSignature();
		signature.PushBack(Signature("Actions", Datum::DatumType::Table, 1, 1));

		return signature;
	}

	void ActionList::InitializeSignatures()
	{
		AddNestedScope("Actions");
	}
}
