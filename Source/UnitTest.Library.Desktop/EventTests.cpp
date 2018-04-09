#pragma once
#include "pch.h"
#include "CppUnitTest.h"
#define  _CRTDBG_MAP_ALLOC
#include <string>
#include <stdlib.h>
#include <crtdbg.h>
#include <exception>
#include <functional>
#include <fstream>
#include "Entity.h"
#include "Sector.h"
#include "World.h"
#include "AbstractFactory.h"
#include "ConcreteFactory.h"
#include "JsonParseHelper.h"
#include "JsonParseMaster.h"
#include "JsonParseHelperTable.h"
#include "TestEntity.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionListIf.h"
#include "TestAction.h"
#include "JsonParseHelperAction.h"
#include "ActionExpression.h"
#include "ActionDestroyAction.h"
#include "Graveyard.h"
#include "ActionCreateAction.h"
#include "Event.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{

	TEST_MODULE_INITIALIZE(OverallInitialization)
	{
		
	}

	TEST_CLASS(EventTests)
	{
	private:
		/*World ContrivedWorld;
		Sector ContrivedSector;
		Entity ContrivedEntity;
		TestEntity ContrivedTestEntity;
		ActionList List1;
		ActionListIf If1;
		ActionExpression XX;
		ActionDestroyAction ContrivedDestroyAction;
		ActionCreateAction ContrivedCreateAction;*/
	public:
		static _CrtMemState sStartMemState;
		/// <summary>
		/// Get Memory Snapshot
		/// </summary>
		TEST_METHOD_INITIALIZE(Initialize)
		{
			//Event<World>::ReserveSpace(10);

			TypeManager::RegisterType(World::TypeIdClass(), World::GetSignature());
			TypeManager::RegisterType(Sector::TypeIdClass(), Sector::GetSignature());
			TypeManager::RegisterType(Entity::TypeIdClass(), Entity::GetSignature());
			TypeManager::RegisterType(TestEntity::TypeIdClass(), TestEntity::GetSignature());
			TypeManager::RegisterType(Action::TypeIdClass(), Action::GetSignature());
			TypeManager::RegisterType(ActionList::TypeIdClass(), ActionList::GetSignature());
			TypeManager::RegisterType(ActionListIf::TypeIdClass(), ActionListIf::GetSignature());
			TypeManager::RegisterType(ActionExpression::TypeIdClass(), ActionExpression::GetSignature());
			TypeManager::RegisterType(ActionDestroyAction::TypeIdClass(), ActionDestroyAction::GetSignature());
			TypeManager::RegisterType(ActionCreateAction::TypeIdClass(), ActionCreateAction::GetSignature());
			TypeManager::RegisterType(TestAction::TypeIdClass(), TestAction::GetSignature());

#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		/// <summary>
		/// Check for leak at the end of the method
		/// </summary>
		TEST_METHOD_CLEANUP(Cleanup)
		{
			Event<World>::ShrinkToFitSubscriberList();
#ifdef _DEBUG
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				_CrtDumpMemoryLeaks();
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(EventInitialization)
		{
			CreateConcreteFactory(TestEntity, Entity);

			World W1("W1");
			Event<World> E1(W1);
			Sector *S1 = W1.CreateSector("S1");
			TestEntity *TE1 = static_cast<TestEntity*>(S1->CreateEntity("TestEntity", "E1"));

			Event<World>::Subscribe(*TE1);
			Event<World>::UnsubscribeAll();

			W1.Update();

			Event<World>::ShrinkToFitSubscriberList();

			AbstractFactory<Entity>::ClearFactories();
		}

		TEST_METHOD(Enqueue)
		{
			CreateConcreteFactory(TestEntity, Entity);

			World W1("W1");
			Sector *S1 = W1.CreateSector("S1");
			TestEntity *TE1 = static_cast<TestEntity*>(S1->CreateEntity("TestEntity", "E1"));

			std::shared_ptr<Event<World>> Event1 = std::make_shared<Event<World>>(W1);
			std::shared_ptr<Event<World>> Event2 = std::make_shared<Event<World>>(W1);
			std::shared_ptr<Event<World>> Event3 = std::make_shared<Event<World>>(W1);
			std::shared_ptr<Event<World>> Event4 = std::make_shared<Event<World>>(W1);
			
			W1.Update();

			W1.GetEventQueue().Enqueue(Event1, W1.GetWorldState().GetGameTime(), std::chrono::seconds(10));
			W1.GetEventQueue().Enqueue(Event2, W1.GetWorldState().GetGameTime(), std::chrono::seconds(100));
			W1.GetEventQueue().Enqueue(Event3, W1.GetWorldState().GetGameTime(), std::chrono::seconds(100));
			W1.GetEventQueue().Enqueue(Event4, W1.GetWorldState().GetGameTime(), std::chrono::seconds(100));

			Event<World>::Subscribe(*TE1);

			W1.Update();

			Assert::AreNotEqual("Event Recieved"s, W1.GetName());
			Assert::AreEqual(4U, W1.GetEventQueue().Length());

			auto endTime = std::chrono::high_resolution_clock::now() + std::chrono::seconds(11);

			while (W1.GetWorldState().GetGameTime().CurrentTime() < endTime)
			{
				W1.Update();
			}

			Assert::AreEqual("Event Recieved"s, TE1->GetName());
			Assert::AreEqual(3U, W1.GetEventQueue().Length());

			W1.GetEventQueue().Clear();
			Assert::AreEqual(0U, W1.GetEventQueue().Length());
			Assert::AreEqual(true, W1.GetEventQueue().IsEmpty());

			Event<World>::UnsubscribeAll();
			Event<World>::ShrinkToFitSubscriberList();

			AbstractFactory<Entity>::ClearFactories();
		}

		TEST_METHOD(Send)
		{
			CreateConcreteFactory(TestEntity, Entity);

			World W1("W1");
			Sector *S1 = W1.CreateSector("S1");
			TestEntity *TE1 = static_cast<TestEntity*>(S1->CreateEntity("TestEntity", "E1"));

			std::shared_ptr<Event<World>> Event1 = std::make_shared<Event<World>>(W1);
			std::shared_ptr<Event<World>> Event2 = std::make_shared<Event<World>>(W1);
			std::shared_ptr<Event<World>> Event3 = std::make_shared<Event<World>>(W1);
			std::shared_ptr<Event<World>> Event4 = std::make_shared<Event<World>>(W1);

			W1.Update();

			W1.GetEventQueue().Enqueue(Event1, W1.GetWorldState().GetGameTime(), std::chrono::seconds(10));
			W1.GetEventQueue().Enqueue(Event2, W1.GetWorldState().GetGameTime(), std::chrono::seconds(100));
			W1.GetEventQueue().Enqueue(Event3, W1.GetWorldState().GetGameTime(), std::chrono::seconds(100));
			W1.GetEventQueue().Enqueue(Event4, W1.GetWorldState().GetGameTime(), std::chrono::seconds(100));

			Event<World>::Subscribe(*TE1);

			W1.Update();

			Assert::AreNotEqual("Event Recieved"s, W1.GetName());
			Assert::AreEqual(4U, W1.GetEventQueue().Length());

			W1.GetEventQueue().Send(Event3);

			Assert::AreEqual("Event Recieved"s, TE1->GetName());
			Assert::AreEqual(3U, W1.GetEventQueue().Length());

			Event<World>::UnsubscribeAll();
			Event<World>::ShrinkToFitSubscriberList();

			AbstractFactory<Entity>::ClearFactories();
		}

	};
	_CrtMemState EventTests::sStartMemState;
}