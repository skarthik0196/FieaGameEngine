#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	class ActionCreateAction : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action)
	public:
		ActionCreateAction();
		ActionCreateAction(const ActionCreateAction& rhs);
		ActionCreateAction(ActionCreateAction&& rhs);
		~ActionCreateAction();

		ActionCreateAction& operator=(const ActionCreateAction& rhs);
		ActionCreateAction& operator=(ActionCreateAction&& rhs);

		void Update();
	private:
		void InitializeSignatures();
		void UpdateExternalStorage();
		std::string ActionClassName;
		std::string ActionInstanceName;
	};
}

