#include "pch.h"
#include "TestEntity.h"
#include "EventPublisher.h"
#include "Event.h"
#include "World.h"
#include "Action.h"
#include <cassert>

using namespace FieaGameEngine;

RTTI_DEFINITIONS(TestEntity)

TestEntity::TestEntity() : Entity(TypeIdInstance())
{
	//InitializeSignatures();
}

TestEntity::TestEntity(const std::string& name) : Entity(TypeIdInstance(), name)
{
	//InitializeSignatures();
}

TestEntity::TestEntity(const TestEntity& rhs) : Entity(rhs), Health(rhs.Health)
{
	//UpdateExternalStorage();
}

TestEntity::TestEntity(TestEntity&& rhs) : Health(std::move(rhs.Health))
{
	//UpdateExternalStorage();
}

TestEntity::~TestEntity()
{

}

TestEntity & TestEntity::operator=(const TestEntity& rhs)
{
	if (this != &rhs)
	{
		Entity::operator=(rhs);
		Health = rhs.Health;
		//UpdateExternalStorage();
	}

	return *this;
}

TestEntity & TestEntity::operator=(TestEntity&& rhs)
{
	if (this != &rhs)
	{
		Entity::operator=(std::move(rhs));
		Health = std::move(rhs.Health);
		//UpdateExternalStorage();
	}

	return *this;
}

void TestEntity::Update(WorldState& worldState)
{
	worldState.CurrentEntity = this;

	Datum& actions = GetActions();
	for (uint32_t i = 0; i < actions.Length(); ++i)
	{
		assert(actions.Get<Scope*>(i)->Is(Action::TypeIdClass()));
		static_cast<Action*>(actions.Get<Scope*>(i))->Update(worldState);
	}

}

void TestEntity::Notify(EventPublisher* event)
{
	assert(event->Is(Event<World>::TypeIdClass()));
	Event <World>* trueEvent = static_cast<Event<World>*>(event);

	auto PayLoad = trueEvent->Message();

	SetName("Event Recieved");
	
}

Vector<Signature> TestEntity::GetSignature()
{
	Vector<Signature> signature = Entity::GetSignature();

	signature.PushBack(Signature("Health", Datum::DatumType::Integer, offsetof(TestEntity, Health), 1));

	return signature;
}

void TestEntity::InitializeSignatures()
{
	AddExternalAttribute("Health", &Health, 1);
}

void TestEntity::UpdateExternalStorage()
{
	UpdateStorage("Health", &Health, 1);
}
