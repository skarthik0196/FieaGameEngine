#include "pch.h"
#include "ActionCreateAction.h"

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

	//Need to Create Action and add to Container
	void ActionCreateAction::Update()
	{

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
