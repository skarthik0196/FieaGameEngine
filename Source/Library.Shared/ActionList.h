#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	class ActionList : public Action
	{
	public:
		ActionList();
		ActionList(const ActionList& rhs);
		ActionList(ActionList&& rhs);
		virtual ~ActionList();

		ActionList& operator=(const ActionList& rhs);
		ActionList& operator=(ActionList&& rhs);

		Datum& GetActions();

		void Update(WorldState& worldState);
	private:
		void InitializeSignatures();
	};
}

