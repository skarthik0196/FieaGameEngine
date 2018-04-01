#pragma once
#include "ActionList.h"

namespace FieaGameEngine
{
	class ActionListIf : public ActionList
	{
		RTTI_DECLARATIONS(ActionListIf, ActionList)

	public:
		ActionListIf();
		ActionListIf(const std::string& name);
		ActionListIf(const ActionListIf& rhs);
		ActionListIf(ActionListIf&& rhs);
		~ActionListIf();

		ActionList& operator=(const ActionListIf& rhs);
		ActionList& operator=(ActionListIf&& rhs);

		void SetConditionValue(const int32_t& newValue);
		void SetIfBlock (Action& ifAction);
		void SetElseBlock(Action& elseAction);

		void Update(WorldState& worldState);
			
	private:
		void InitializeSignatures();
		void UpdateExternalStorage();
		int32_t ConditionValue;
	};
}

