#include "pch.h"
#include "TestAction.h"
#include "Entity.h"

using namespace FieaGameEngine;

RTTI_DEFINITIONS(TestAction)

TestAction::TestAction() : Action()
{

}

TestAction::~TestAction()
{

}

void TestAction::Update(FieaGameEngine::WorldState & worldState)
{
	worldState.CurrentEntity->SetName("EE");
}
