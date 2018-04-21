#pragma once
#include "Entity.h"
#include "EventSubscriber.h"
#include "WorldState.h"
#include <thread>
#include <future>
#include <mutex>

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

	static FieaGameEngine::Vector<FieaGameEngine::Signature> GetSignature();

private:
	void InitializeSignatures();
	void UpdateExternalStorage();
	int32_t Health;
	int32_t EventCount;
	std::mutex Mutex;

};

