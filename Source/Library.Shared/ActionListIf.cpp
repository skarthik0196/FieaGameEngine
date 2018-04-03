#include "pch.h"
#include "ActionListIf.h"


namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionListIf)

	ActionListIf::ActionListIf() : ActionList()
	{
		ActionListIf::InitializeSignatures();
	}

	ActionListIf::ActionListIf(const std::string& name) : ActionList(name)
	{
		ActionListIf::InitializeSignatures();
	}

	ActionListIf::ActionListIf(const ActionListIf& rhs) : ActionList(rhs)
	{
		ActionListIf::UpdateExternalStorage();
	}

	ActionListIf::ActionListIf(ActionListIf&& rhs) : ActionList(std::move(rhs))
	{
		ActionListIf::UpdateExternalStorage();
	}

	ActionListIf::~ActionListIf()
	{

	}

	ActionList & ActionListIf::operator=(const ActionListIf& rhs)
	{
		if (this != &rhs)
		{
			ActionList::operator=(rhs);
			ConditionValue = rhs.ConditionValue;
			UpdateExternalStorage();
		}
		return *this;
	}

	ActionList & ActionListIf::operator=(ActionListIf && rhs)
	{
		if (this != &rhs)
		{
			ActionList::operator=(std::move(rhs));
			ConditionValue = std::move(rhs.ConditionValue);
			UpdateExternalStorage();
		}
		return *this;
	}

	void ActionListIf::SetConditionValue(const int32_t& newValue)
	{
		ConditionValue = newValue;
	}

	void ActionListIf::SetIfBlock(Action& ifAction)
	{
		Datum& ifBlock = (*this)["ThenBlock"];
		
		if (ifBlock.Length() == 0)
		{
			Adopt(ifAction, "ThenBlock");
		}
		else
		{
			ifBlock.Set(&ifAction, 0);
		}
		
	}

	void ActionListIf::SetElseBlock(Action& elseAction)
	{
		Datum& elseBlock = (*this)["ElseBlock"];
		if (elseBlock.Length() == 0)
		{
			Adopt(elseAction, "ElseBlock");
		}
		else
		{
			elseBlock.Set(&elseAction, 0);
		}
	}

	void ActionListIf::Update(WorldState& worldState)
	{
		worldState.CurrentAction = this;

		Datum& datum = (*this)["Evaluation"];

		if (datum.Length() > 0)
		{
			static_cast<Action*>(&(datum[0]))->Update(worldState);
		}

		if (ConditionValue == 1)
		{
			static_cast<Action*>(&(*this)["ThenBlock"][0])->Update(worldState);
		}
		else
		{
			static_cast<Action*>(&(*this)["ElseBlock"][0])->Update(worldState);
		}
	}

	void ActionListIf::SetEvaluationBlock(Action & evaluationAction)
	{
		Datum& evaluationBlock = (*this)["Evaluation"];
		if (evaluationBlock.Length() == 0)
		{
			Adopt(evaluationAction, "Evaluation");
		}
		else
		{
			evaluationBlock.Set(&evaluationAction, 0);
		}
	}

	void ActionListIf::InitializeSignatures()
	{
		AddExternalAttribute("ConditionValue", &ConditionValue, 1);
		AddNestedScope("ThenBlock");
		AddNestedScope("ElseBlock");
		AddNestedScope("Evaluation");
	}
	void ActionListIf::UpdateExternalStorage()
	{
		UpdateStorage("ConditionValue", &ConditionValue, 1);
	}
}
