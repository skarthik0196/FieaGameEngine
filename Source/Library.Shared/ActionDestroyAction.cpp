#include "pch.h"
#include "ActionDestroyAction.h"
#include "Graveyard.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionDestroyAction)

	ActionDestroyAction::ActionDestroyAction() : Action(TypeIdInstance())
	{
		//InitializeSignatures();
	}


	ActionDestroyAction::ActionDestroyAction(const std::string& name) : Action(TypeIdInstance(), name)
	{
		//InitializeSignatures();
	}

	ActionDestroyAction::ActionDestroyAction(const ActionDestroyAction& rhs) : Action(rhs)
	{
		//UpdateExternalStorage();
	}

	ActionDestroyAction::ActionDestroyAction(ActionDestroyAction&& rhs) : Action(std::move(rhs))
	{
		//UpdateExternalStorage();
	}

	ActionDestroyAction::~ActionDestroyAction()
	{

	}

	ActionDestroyAction& ActionDestroyAction::operator=(const ActionDestroyAction& rhs)
	{
		if (this != &rhs)
		{
			Action::operator=(rhs);
			ActionInstanceName = rhs.ActionInstanceName;
			//UpdateExternalStorage();
		}
		return *this;
	}

	ActionDestroyAction& ActionDestroyAction::operator=(ActionDestroyAction&& rhs)
	{
		if (this != &rhs)
		{
			Action::operator=(std::move(rhs));
			ActionInstanceName = std::move(rhs.ActionInstanceName);
			//UpdateExternalStorage();
		}
		return *this;
	}

	//Add to Graveyard here
	void ActionDestroyAction::Update(WorldState& worldState)
	{
		worldState.CurrentAction = this;

		if (!ActionInstanceName.empty())
		{
			Scope *currentScope = this;
			bool isFound = false;
			Datum *datum = currentScope->Search("Actions");

			while (isFound == false && datum != nullptr)
			{
				for (uint32_t i = 0; i < datum->Length(); ++i)
				{
					Scope* actionScope = datum->Get<Scope*>(i);
					if ((*actionScope)["Name"].Get<std::string>() == ActionInstanceName)
					{
						GrimReaper::AddToKillList(*actionScope);
						ActionInstanceName = "";
						isFound = true;
						break;
					}
				}
				currentScope = currentScope->GetParent();
				datum = currentScope->Search("Actions");
			}
		}
	}

	void ActionDestroyAction::SetActionInstanceName(const std::string &actionInstanceName)
	{
		ActionInstanceName = actionInstanceName;
	}

	const std::string & ActionDestroyAction::GetActionInstanceName()
	{
		return ActionInstanceName;
	}

	void ActionDestroyAction::AddSelfToKillList()
	{
		GrimReaper::AddToKillList(*this);
	}

	Vector<Signature> ActionDestroyAction::GetSignature()
	{
		Vector<Signature> signature = Action::GetSignature();
		signature.PushBack(Signature("ActionInstanceName", Datum::DatumType::String, offsetof(ActionDestroyAction, ActionInstanceName), 1));

		return signature;
	}

	void ActionDestroyAction::InitializeSignatures()
	{
		AddExternalAttribute("ActionInstanceName", &ActionInstanceName, 1);
	}

	void ActionDestroyAction::UpdateExternalStorage()
	{
		UpdateStorage("ActionInstanceName", &ActionInstanceName, 1);
	}
}
