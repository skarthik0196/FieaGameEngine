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
		CreateConcreteFactory(TestEntity, Entity);
		Event<World>::ReserveSpace(10);
	}

	TEST_CLASS(EventTests)
	{
	private:
		World ContrivedWorld;
		Sector ContrivedSector;
		Entity ContrivedEntity;
		TestEntity ContrivedTestEntity;
		ActionList List1;
		ActionListIf If1;
		ActionExpression XX;
		ActionDestroyAction ContrivedDestroyAction;
		ActionCreateAction ContrivedCreateAction;
	public:
		static _CrtMemState sStartMemState;
		/// <summary>
		/// Get Memory Snapshot
		/// </summary>
		TEST_METHOD_INITIALIZE(Initialize)
		{
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
			World W1("W1");
			Event<World> E1(W1);
			Sector *S1 = W1.CreateSector("S1");
			TestEntity *TE1 = static_cast<TestEntity*>(S1->CreateEntity("TestEntity", "E1"));

			Event<World>::Subscribe(*TE1);
			Event<World>::UnsubscribeAll();

			W1.Update();

			AbstractFactory<Entity>::ClearFactories();
		}

		TEST_METHOD(Enqueue)
		{
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
		}

		TEST_METHOD(Send)
		{
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
		}

	};
	_CrtMemState EventTests::sStartMemState;
}