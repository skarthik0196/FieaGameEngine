#pragma once
#include "Action.h"

class TestAction : public FieaGameEngine::Action
{
	RTTI_DECLARATIONS(TestAction, Action)

public:
	TestAction();
	~TestAction();

	void Update(FieaGameEngine::WorldState& worldState);
};

