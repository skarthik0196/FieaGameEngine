#include "pch.h"
#include "ActionCreateAction.h"
#include "AbstractFactory.h"
#include "ConcreteFactory.h"
#include "Entity.h"
#include "ActionList.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionCreateAction)
	ActionCreateAction::ActionCreateAction() : Action()
	{
		InitializeSignatures();
	}

	ActionCreateAction::ActionCreateAction(const ActionCreateAction& rhs) :Action(rhs)
	{
	}

	ActionCreateAction::ActionCreateAction(ActionCreateAction&& rhs): Action(std::move(rhs))
	{
	}

	ActionCreateAction::~ActionCreateAction()
	{
	}

	ActionCreateAction& ActionCreateAction::operator=(const ActionCreateAction& rhs)
	{
		if (this != &rhs)
		{
			Action::operator=(rhs);
			ActionClassName = rhs.ActionClassName;
			ActionInstanceName = rhs.ActionInstanceName;
			UpdateExternalStorage();
		}

		return *this;
	}

	ActionCreateAction& ActionCreateAction::operator=(ActionCreateAction&& rhs)
	{
		if (this != &rhs)
		{
			Action::operator=(std::move(rhs));
			ActionClassName = std::move(rhs.ActionClassName);
			ActionInstanceName = std::move(rhs.ActionInstanceName);
			UpdateExternalStorage();
		}
		return *this;
	}

	void ActionCreateAction::SetActionClassName(const std::string& actionClassName)
	{
		ActionClassName = actionClassName;
	}

	void ActionCreateAction::SetActionInstanceName(const std::string& actionInstanceName)
	{
		ActionInstanceName = actionInstanceName;
	}

	const std::string & ActionCreateAction::GetActionClassName() const
	{
		return ActionClassName;
	}

	const std::string & ActionCreateAction::GetActionInstanceName() const
	{
		return ActionInstanceName;
	}

	void ActionCreateAction::Update(WorldState& worldState)
	{
		worldState.CurrentAction = this;

		Entity *entityParent = GetParent()->As<Entity>();
		if (entityParent != nullptr)
		{
			entityParent->CreateAction(ActionClassName, ActionInstanceName);
		}
		else
		{
			ActionList *aListParent = GetParent()->As<ActionList>();
			if (aListParent != nullptr)
			{
				aListParent->CreateAction(ActionClassName, ActionInstanceName);
			}
		}
	}

	void ActionCreateAction::InitializeSignatures()
	{
		AddExternalAttribute("ActionClassName", &ActionClassName, 1);
		AddExternalAttribute("ActionInstanceName", &ActionInstanceName, 1);
	}

	void ActionCreateAction::UpdateExternalStorage()
	{
		UpdateStorage("ActionClassName", &ActionClassName, 1);
		UpdateStorage("ActionInstanceName", &ActionInstanceName, 1);
	}
}
