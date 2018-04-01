#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action)

	public:
		ActionList();
		ActionList(const std::string& name);
		ActionList(const ActionList& rhs);
		ActionList(ActionList&& rhs);
		virtual ~ActionList();

		ActionList& operator=(const ActionList& rhs);
		ActionList& operator=(ActionList&& rhs);

		Datum& GetActions();

		Action* CreateAction(const std::string& className, const std::string& instanceName);
		void AppendAction(Action& action);
		void Update(WorldState& worldState);
	private:
		void InitializeSignatures();
	};
}

