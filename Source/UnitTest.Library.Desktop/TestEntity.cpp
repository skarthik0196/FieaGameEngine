#include "pch.h"
#include "TestEntity.h"
#include "EventPublisher.h"
#include "Event.h"
#include "World.h"

using namespace FieaGameEngine;

RTTI_DEFINITIONS(TestEntity)

TestEntity::TestEntity() : Entity()
{
	InitializeSignatures();
}

TestEntity::TestEntity(const std::string& name) : Entity(name)
{
	InitializeSignatures();
}

TestEntity::TestEntity(const TestEntity& rhs) : Entity(rhs), Health(rhs.Health)
{
	UpdateExternalStorage();
}

TestEntity::TestEntity(TestEntity&& rhs) : Health(std::move(rhs.Health))
{
	UpdateExternalStorage();
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
		UpdateExternalStorage();
	}

	return *this;
}

TestEntity & TestEntity::operator=(TestEntity&& rhs)
{
	if (this != &rhs)
	{
		Entity::operator=(std::move(rhs));
		Health = std::move(rhs.Health);
		UpdateExternalStorage();
	}

	return *this;
}

void TestEntity::Update(WorldState& worldState)
{
	worldState.CurrentEntity = this;
}

void TestEntity::Notify(EventPublisher* event)
{
	event;
	Event <World>* trueEvent = event->As<Event<World>>();
	auto PayLoad = trueEvent->Message();

	SetName("Event Recieved");
	
}

void TestEntity::InitializeSignatures()
{
	AddExternalAttribute("Health", &Health, 1);
}

void TestEntity::UpdateExternalStorage()
{
	UpdateStorage("Health", &Health, 1);
}
