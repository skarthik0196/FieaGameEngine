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

		TEST_METHOD(ParseActionsFromFile)
		{
			CreateConcreteFactory(ActionList, Action);
			CreateConcreteFactory(ActionListIf, Action);
			CreateConcreteFactory(Entity, Entity)
			CreateConcreteFactory(TestEntity, Entity)
			CreateConcreteFactory(TestAction, Action);
			CreateConcreteFactory(ActionExpression, Action);

			std::string FileName = "Scripts/EntityParseTest.json"s;

			World W1;
			JsonParseHelperTable::TableSharedData SharedScope(W1);
			JsonParseMaster Master1(SharedScope);

			JsonParseHelperTable TableHelper;
			Master1.AddHelper(TableHelper);
			Master1.ParseFromFile(FileName);

			Sector *S1 = W1.GetSectors().Get<Scope*>()->As<Sector>();
			Entity *E1 = S1->GetEntities().Get<Scope*>()->As<Entity>();

			JsonParseHelperAction ActionHelper;
			JsonParseHelperAction::EntitySharedData SharedEntity(*E1);
			Master1.SetSharedData(SharedEntity);
			Master1.AddHelper(ActionHelper);

			Master1.ParseFromFile("Scripts/ActionParseTest.json"s);

			W1.Update();

			Assert::AreEqual("EE"s, E1->GetName());
			Assert::AreEqual(2U, E1->GetActions().Length());

			W1.Update();

			ActionList *AList1 = E1->GetActions().Get<Scope*>(0)->As<ActionList>();
			Action *ActionIf = E1->GetActions().Get<Scope*>(1)->As<ActionList>();

			if (AList1 != nullptr)
			{
				Assert::AreEqual(1U, AList1->GetActions().Length());
				ActionExpression *Expr1 = AList1->GetActions().Get<Scope*>()->As<ActionExpression>();
				Assert::AreEqual(10.0f, (*Expr1)["Result"].Get<float>(0));

				Assert::AreEqual(0, (*ActionIf)["ConditionValue"].Get<int32_t>(0));
			}

			AbstractFactory<Entity>::ClearFactories();
			AbstractFactory<Action>::ClearFactories();
		}

		TEST_METHOD(RPNEvaluationCheck)
		{
			CreateConcreteFactory(ActionExpression, Action);

			WorldState worldState;
			ActionExpression Exp1("Exp1"s);

			Exp1.SetRPN("15 7 1 1 + - / 3 * 2 1 1 + + -");

			Exp1.Update(worldState);

			Assert::AreEqual(5.0f, Exp1["Result"].Get<float>(0));

			Exp1["Health"] = 100;
			Assert::AreEqual(100, Exp1["Health"].Get<int32_t>(0));

			Exp1.SetRPN("Health Health 10 - =");

			Exp1.Update(worldState);

			Assert::AreEqual(90.0f, Exp1["Result"].Get<float>(0));
			Assert::AreEqual(90, Exp1["Health"].Get<int32_t>(0));

			AbstractFactory<Action>::ClearFactories();
		}

		TEST_METHOD(ActionDeleteAction)
		{
			CreateConcreteFactory(ActionList, Action);
			CreateConcreteFactory(TestAction, Action);
			CreateConcreteFactory(Entity, Entity)
			CreateConcreteFactory(ActionDestroyAction, Action)
			CreateConcreteFactory(ActionListIf,Action)

			World W1("W1");
			Entity *E1 = W1.CreateSector("S1")->CreateEntity("Entity", "E1");
			E1;
			ActionList *AList1 = static_cast<ActionList*>(E1->CreateAction("ActionList", "AList1"));
			Assert::AreEqual("AList1"s, AList1->GetName());

			AList1->CreateAction("TestAction", "TestA1");
			ActionDestroyAction *D1 = static_cast<ActionDestroyAction*>(AList1->CreateAction("ActionDestroyAction", "Destroy1"));
			D1->SetActionInstanceName("TestA1"s);
			Assert::AreEqual(2U, AList1->GetActions().Length());

			W1.Update();

			Assert::AreEqual(1U, GrimReaper::KillListSize());
			
			W1.Update();

			Assert::AreEqual(0U, GrimReaper::KillListSize());
			Assert::AreEqual(1U, AList1->GetActions().Length());

			AbstractFactory<Entity>::ClearFactories();
			AbstractFactory<Action>::ClearFactories();
		}

		TEST_METHOD(CreateActionClass)
		{
			CreateConcreteFactory(ActionList, Action);
			CreateConcreteFactory(TestAction, Action);
			CreateConcreteFactory(Entity, Entity)
			CreateConcreteFactory(ActionCreateAction, Action)
			CreateConcreteFactory(ActionListIf, Action)

			World W1("W1");
			Entity *E1 = W1.CreateSector("S1")->CreateEntity("Entity", "E1");
			E1;
			ActionList *AList1 = static_cast<ActionList*>(E1->CreateAction("ActionList", "AList1"));
			Assert::AreEqual("AList1"s, AList1->GetName());

			ActionCreateAction *D1 = static_cast<ActionCreateAction*>(AList1->CreateAction("ActionCreateAction", "Create1"));
			D1->SetActionClassName("TestAction");
			D1->SetActionInstanceName("Test1");

			Assert::AreEqual(1U, AList1->GetActions().Length());

			W1.Update();

			Assert::AreEqual(2U, AList1->GetActions().Length());
			Assert::AreEqual("EE"s, E1->GetName());

			AbstractFactory<Entity>::ClearFactories();
			AbstractFactory<Action>::ClearFactories();
		}



	};
	_CrtMemState ActionTests::sStartMemState;
}