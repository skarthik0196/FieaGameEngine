#pragma once
#include "Action.h"

class TestAction : public FieaGameEngine::Action
{
	RTTI_DECLARATIONS(TestAction, Action)

public:
	TestAction();
	~TestAction();

	static FieaGameEngine::Vector<FieaGameEngine::Signature> GetSignature();

	void Update(FieaGameEngine::WorldState& worldState);
};

