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

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionTests)
	{
	private:
		World ContrivedWorld;
		Sector ContrivedSector;
		Entity ContrivedEntity;
		TestEntity ContrivedTestEntity;
		ActionList List1;
		ActionListIf If1;
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

		TEST_METHOD(ActionListInitialization)
		{
			CreateConcreteFactory(ActionList, Action);
			CreateConcreteFactory(ActionListIf, Action);
			CreateConcreteFactory(Entity, Entity)

			World W1("W1");
			Entity *E1 = W1.CreateSector("S1")->CreateEntity("Entity", "E1");
			E1;
			ActionList *AList1 = static_cast<ActionList*>(E1->CreateAction("ActionList", "AList1"));
			Assert::AreEqual("AList1"s, AList1->GetName());

			ActionListIf* ActionIf1 = static_cast<ActionListIf*>(AList1->CreateAction("ActionListIf", "If1"));
			ActionIf1->SetConditionValue(1);
			Assert::AreEqual(ActionIf1->GetName(), std::string("If1"));

			AbstractFactory<Entity>::ClearFactories();
			AbstractFactory<Action>::ClearFactories();
		}

		TEST_METHOD(ActionListIfTrue)
		{
			CreateConcreteFactory(ActionList, Action);
			CreateConcreteFactory(ActionListIf, Action);
			CreateConcreteFactory(Entity, Entity)

			World W1("W1");
			Entity *E1 = W1.CreateSector("S1")->CreateEntity("Entity", "E1");
			E1;
			ActionList *AList1 = static_cast<ActionList*>(E1->CreateAction("ActionList", "AList1"));
			Assert::AreEqual("AList1"s, AList1->GetName());

			ActionListIf* ActionIf1 = static_cast<ActionListIf*>(AList1->CreateAction("ActionListIf", "If1"));
			ActionIf1->SetConditionValue(1);
			Assert::AreEqual(ActionIf1->GetName(), std::string("If1"));

			TestAction *TA = new TestAction();
			ActionIf1->SetIfBlock(*TA);

			Assert::AreEqual("E1"s, E1->GetName());

			W1.Update();

			Assert::AreEqual("EE"s, E1->GetName());

			AbstractFactory<Entity>::ClearFactories();
			AbstractFactory<Action>::ClearFactories();
		}

		TEST_METHOD(ActionListIfFalse)
		{
			CreateConcreteFactory(ActionList, Action);
			CreateConcreteFactory(ActionListIf, Action);
			CreateConcreteFactory(Entity, Entity)

			World W1("W1");
			Entity *E1 = W1.CreateSector("S1")->CreateEntity("Entity", "E1");
			E1;
			ActionList *AList1 = static_cast<ActionList*>(E1->CreateAction("ActionList", "AList1"));
			Assert::AreEqual("AList1"s, AList1->GetName());

			ActionListIf* ActionIf1 = static_cast<ActionListIf*>(AList1->CreateAction("ActionListIf", "If1"));
			ActionIf1->SetConditionValue(0);
			Assert::AreEqual(ActionIf1->GetName(), std::string("If1"));

			TestAction *TA = new TestAction();
			ActionIf1->SetElseBlock(*TA);

			Assert::AreEqual("E1"s, E1->GetName());

			W1.Update();

			Assert::AreEqual("EE"s, E1->GetName());

			AbstractFactory<Entity>::ClearFactories();
			AbstractFactory<Action>::ClearFactories();
		}

	};
	_CrtMemState ActionTests::sStartMemState;
}