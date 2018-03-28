#pragma once
#include "Entity.h"

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

	void Update();

private:
	void InitializeSignatures();
	void UpdateExternalStorage();
	int32_t Health;

};

