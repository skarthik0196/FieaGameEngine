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
#include "ReactionAttributed.h"
#include "ActionEvent.h"
#include "EventMessageAttributed.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ReactionTests)
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
			TypeManager::RegisterType(Reaction::TypeIdClass(), Reaction::GetSignature());
			TypeManager::RegisterType(EventMessageAttributed::TypeIdClass(), EventMessageAttributed::GetSignature());
			TypeManager::RegisterType(ActionEvent::TypeIdClass(), ActionEvent::GetSignature());
			TypeManager::RegisterType(ReactionAttributed::TypeIdClass(), ReactionAttributed::GetSignature());

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
			Event<EventMessageAttributed>::ShrinkToFitSubscriberList();

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

		TEST_METHOD(Initialization)
		{
			EventMessageAttributed E1;
			Event<EventMessageAttributed> Event1(E1);
			ReactionAttributed R1("R1");
			ActionEvent A1("asf");
			Event<EventMessageAttributed>::ShrinkToFitSubscriberList();
		}

		TEST_METHOD(CreateReaction)
		{
			CreateConcreteFactory(ReactionAttributed, Reaction);

			World W1("W1");
			ReactionAttributed* R1 = (W1.CreateReaction("ReactionAttributed"))->As<ReactionAttributed>();
			R1;

			Assert::IsTrue(R1 == &W1["Reactions"][0]);

			AbstractFactory<Reaction>::ClearFactories();
		}

		TEST_METHOD(EnqueueActionEvent)
		{
			CreateConcreteFactory(ReactionAttributed, Reaction);
			CreateConcreteFactory(ActionEvent, Action);
			CreateConcreteFactory(ActionListIf, Action);
			CreateConcreteFactory(TestAction, Action);
			CreateConcreteFactory(Entity, Entity);

			World W1("W1"s);
			ReactionAttributed* R1 = (W1.CreateReaction("ReactionAttributed"s))->As<ReactionAttributed>();
			R1->SetSubType("Integer"s);

			Assert::AreEqual("Integer"s, R1->GetSubType());

			ActionListIf* If1 = R1->CreateAction("ActionListIf", "If1")->As<ActionListIf>();
			If1->SetConditionValue(1);
			ActionExpression* Ta = new ActionExpression();
			Ta->SetRPN("1 1 +"s);
			If1->SetIfBlock(*Ta);

			Assert::IsTrue(R1 == &W1["Reactions"][0]);

			Entity *E1 = W1.CreateSector("S1"s)->CreateEntity("Entity"s, "E1"s);
			ActionEvent *A1 = E1->CreateAction("ActionEvent"s, "A1"s)->As<ActionEvent>();
			A1->AppendAuxillaryAttribute("Aux1"s) = 3;

			Datum& d = A1->AppendAuxillaryAttribute("Aux2"s);
			d.SetType(Datum::DatumType::Table); 
			d.PushBack(new Scope());

			A1->SetDelay(std::chrono::milliseconds(0));
			A1->SetSubType("Integer"s);

			Assert::IsTrue(std::chrono::milliseconds(0) == A1->GetDelay());
			Assert::AreEqual("Integer"s, A1->GetSubType());

			W1.Update();

			Assert::AreEqual(1U, W1.GetEventQueue().Length());

			W1.Update();

			Assert::AreEqual(2.00f, (*Ta)["Result"s].Get<float>(0));

			AbstractFactory<Entity>::ClearFactories();
			AbstractFactory<Reaction>::ClearFactories();
			AbstractFactory<Action>::ClearFactories();
		}

		TEST_METHOD(ParseReaction)
		{
			CreateConcreteFactory(ReactionAttributed, Reaction);
			CreateConcreteFactory(ActionEvent, Action);
			CreateConcreteFactory(ActionListIf, Action);
			CreateConcreteFactory(ActionExpression, Action)
			CreateConcreteFactory(TestAction, Action);
			CreateConcreteFactory(Entity, Entity);

			std::string FileName = "Scripts/ReactionParse.json"s;

			World W1("W1"s);

			Entity *E1 = W1.CreateSector("S1"s)->CreateEntity("Entity"s, "E1"s);

			JsonParseHelperAction ActionHelper;
			JsonParseHelperAction::EntitySharedData SharedEntity(*E1);

			JsonParseMaster Master1(SharedEntity);

			Master1.AddHelper(ActionHelper);

			Master1.ParseFromFile(FileName);

			ActionEvent *A1 = E1->GetActions().Get<Scope*>(0)->As<ActionEvent>();
			Assert::IsTrue(A1->GetParent() == E1);

			W1.Update();

			Assert::AreEqual(1U, W1.GetEventQueue().Length());

			W1.Update();

			ActionExpression *Ta = (&W1.GetReactions()[0]["Actions"s][0]["ThenBlock"s][0])->As<ActionExpression>();

			Assert::AreEqual(2.00f, (*Ta)["Result"s].Get<float>(0));

			AbstractFactory<Entity>::ClearFactories();
			AbstractFactory<Reaction>::ClearFactories();
			AbstractFactory<Action>::ClearFactories();
		}
	};
	_CrtMemState ReactionTests::sStartMemState;
}