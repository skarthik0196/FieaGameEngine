#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	class ActionExpression : public Action
	{
		RTTI_DECLARATIONS(ActionExpression, Action)
	public:
		ActionExpression();
		~ActionExpression();
	};
}

