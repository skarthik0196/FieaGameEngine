#include "pch.h"
#include "TestEntity.h"
#include "EventPublisher.h"
#include "Event.h"
#include "World.h"
#include "Action.h"
#include <cassert>

using namespace FieaGameEngine;

RTTI_DEFINITIONS(TestEntity)

TestEntity::TestEntity() : Entity(TypeIdInstance()), EventCount(0)
{
	//InitializeSignatures();
}

TestEntity::TestEntity(const std::string& name) : Entity(TypeIdInstance(), name), EventCount(0)
{
	//InitializeSignatures();
}

TestEntity::TestEntity(const TestEntity& rhs) : Entity(rhs), Health(rhs.Health), EventCount(0)
{
	//UpdateExternalStorage();
}

TestEntity::TestEntity(TestEntity&& rhs) : Health(std::move(rhs.Health)), EventCount(0)
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
	std::lock_guard<std::mutex> lock(Mutex);

	Event<World>* trueEventWorld = event->As<Event<World>>();
	Event<int32_t>* trueEventInt = event->As<Event<int32_t>>();
	Event<float>* trueEventFloat = event->As<Event<float>>();
	Event<char>* trueEventChar = event->As<Event<char>>();

	EventCount++;
	trueEventInt;
	trueEventFloat;
	trueEventChar;

	if (trueEventWorld != nullptr)
	{
		auto PayLoad = trueEventWorld->Message();
		SetName("Event Recieved");
		
	}
	else if (trueEventInt != nullptr)
	{
		World *ParentWorld = static_cast<World*>(GetParent()->GetParent());
		float x = 3;
		ParentWorld->GetEventQueue().Enqueue(std::make_shared<Event<float>>(x), ParentWorld->GetWorldState().GetGameTime(), std::chrono::milliseconds(0));
	}
	//else if (trueEventFloat != nullptr)
	//{
	//	EventCount++;
	//}
	else if (trueEventChar != nullptr)
	{
		Event<char>::Unsubscribe(*this);
	}
	
}

Vector<Signature> TestEntity::GetSignature()
{
	Vector<Signature> signature = Entity::GetSignature();

	signature.PushBack(Signature("Health", Datum::DatumType::Integer, offsetof(TestEntity, Health), 1));
	signature.PushBack(Signature("EventCount", Datum::DatumType::Integer, offsetof(TestEntity, EventCount), 1));

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
