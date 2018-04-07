#pragma once
#include "Entity.h"
#include "EventSubscriber.h"
#include "WorldState.h"

class TestEntity : public FieaGameEngine::Entity
{
	RTTI_DECLARATIONS(TestEntity, Entity)

public:
	TestEntity();
	TestEntity(const std::string& name);
	TestEntity(const TestEntity& rhs);
	TestEntity(TestEntity&& rhs);
	~TestEntity();

	TestEntity& operator=(const TestEntity& rhs);
	TestEntity& operator=(TestEntity&& rhs);

	void Update(FieaGameEngine::WorldState& worldState);

	void Notify(FieaGameEngine::EventPublisher* event);

private:
	void InitializeSignatures();
	void UpdateExternalStorage();
	int32_t Health;

};

