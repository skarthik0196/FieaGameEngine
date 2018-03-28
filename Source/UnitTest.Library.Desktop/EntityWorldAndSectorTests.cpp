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

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EntitySectorWorldTests)
	{
	private:
		World ContrivedWorld;
		Sector ContrivedSector;
		Entity ContrivedEntity;
		TestEntity ContrivedTestEntity;
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

		TEST_METHOD(Initialization)
		{
			CreateConcreteFactory(Entity, Entity)

			World W1("W1");
			Assert::AreEqual(std::string("W1"), W1.GetName());

			Sector* S1 = W1.CreateSector("S1");
			Assert::AreEqual((*S1)["Name"].Get<std::string>(0), S1->GetName());
			Assert::AreEqual(std::string("S1"), S1->GetName());

			Entity* E1 = S1->CreateEntity("Entity", "E1");
			Assert::AreEqual(std::string("E1"), E1->GetName());

			AbstractFactory<Entity>::ClearFactories();
		}

		TEST_METHOD(Update)
		{
			CreateConcreteFactory(Entity, Entity)

			World W1("World1");
			Sector *S1 = W1.CreateSector("S1");
			Sector *S2 = W1.CreateSector("S2");

			S1->CreateEntity("Entity", "E1");
			S1->CreateEntity("Entity", "E2");

			S2->CreateEntity("Entity", "E3");

			W1.Update();
			
			Assert::IsTrue(W1.GetWorldState().CurrentEntity == S2->GetEntities().Get<Scope*>(0));

			AbstractFactory<Entity>::ClearFactories();
		}

		TEST_METHOD(GetName)
		{
			CreateConcreteFactory(Entity, Entity)

			World W1("W1");
			Assert::AreEqual(std::string("W1"), W1.GetName());

			Sector* S1 = W1.CreateSector("S1");
			Assert::AreEqual((*S1)["Name"].Get<std::string>(0), S1->GetName());
			Assert::AreEqual(std::string("S1"), S1->GetName());

			Entity* E1 = S1->CreateEntity("Entity", "E1");
			Assert::AreEqual(std::string("E1"), E1->GetName());

			AbstractFactory<Entity>::ClearFactories();
		}

		TEST_METHOD(GetSectorGetWorld)
		{
			CreateConcreteFactory(Entity, Entity)

			World W1("W1");
			Assert::AreEqual(std::string("W1"), W1.GetName());

			Sector* S1 = W1.CreateSector("S1");
			Assert::AreEqual((*S1)["Name"].Get<std::string>(0), S1->GetName());
			Assert::AreEqual(std::string("S1"), S1->GetName());
			Assert::IsTrue(S1->GetWorld() == &W1);

			Entity* E1 = S1->CreateEntity("Entity", "E1");
			Assert::AreEqual(std::string("E1"), E1->GetName());
			Assert::IsTrue(E1->GetSector() == S1);

			AbstractFactory<Entity>::ClearFactories();
		}

		TEST_METHOD(Entities)
		{
			CreateConcreteFactory(Entity, Entity)

			World W1("World1");
			Sector *S1 = W1.CreateSector("S1");

			S1->CreateEntity("Entity", "E1");
			Entity* E2 = S1->CreateEntity("Entity", "E2");
			S1->CreateEntity("Entity", "E3");

			Datum& Entities = S1->GetEntities();
			Assert::AreEqual(3U, Entities.Length());
			Assert::IsTrue(Entities.Get<Scope*>(1) == E2);

			AbstractFactory<Entity>::ClearFactories();
		}

		TEST_METHOD(Sectors)
		{
			CreateConcreteFactory(Entity, Entity)

			World W1("World1");
			Sector *S1 = W1.CreateSector("S1");
			W1.CreateSector("S2");
			S1->CreateEntity("Entity", "E1");
			S1->CreateEntity("Entity", "E2");
			S1->CreateEntity("Entity", "E3");

			Datum& Sectors = W1.GetSectors();
			Assert::AreEqual(2U, Sectors.Length());
			Assert::IsTrue(S1 == Sectors.Get<Scope*>(0));

			AbstractFactory<Entity>::ClearFactories();
		}

		TEST_METHOD(Parsing)
		{
			CreateConcreteFactory(Entity, Entity)
			CreateConcreteFactory(TestEntity, Entity)

			std::string FileName = "Scripts/EntityParseTest.json";

			World W1;
			JsonParseHelperTable::TableSharedData SharedScope(W1);
			JsonParseMaster Master1(SharedScope);

			JsonParseHelperTable TableHelper;
			Master1.AddHelper(TableHelper);
			Master1.ParseFromFile(FileName);
			
			Assert::AreEqual((W1)["Weather"].Get<std::string>(0), std::string("Sunny"));

			Assert::AreEqual(1U, W1.GetSectors().Length());
			Sector *S1 = W1.GetSectors().Get<Scope*>(0)->As<Sector>();
			Assert::AreEqual((*S1)["NPC's"].Get<int32_t>(0), 8);

			Assert::AreEqual(2U, S1->GetEntities().Length());

			Datum& Entities = S1->GetEntities();
			Entity *E1, *E2;

			if ((*Entities.Get<Scope*>(0))["Name"].Get<std::string>(0) == "CharacterStats")
			{
				E2 = (*S1)["Entities"].Get<Scope*>(0)->As<Entity>();
				E1 = (*S1)["Entities"].Get<Scope*>(1)->As<Entity>();
			}
			else
			{
				E2 = (*S1)["Entities"].Get<Scope*>(1)->As<Entity>();
				E1 = (*S1)["Entities"].Get<Scope*>(0)->As<Entity>();
			}
			
			Assert::AreEqual((*E1)["Invisibility"].Get<int32_t>(0), 3);
			Assert::AreEqual((*E1)["Dash"].Get<int32_t>(0), 3);

			AbstractFactory<Entity>::ClearFactories();
		}

		TEST_METHOD(TimeTests)
		{
			World W1;
			W1.SetName("W1");
			W1.Update();
			GameClock& GC = W1.GetWorldState().GetGameClock();
			W1.Update();
			GameTime& GT = W1.GetWorldState().GetGameTime();

			GT.SetElapsedGameTime(std::chrono::milliseconds(100));
			GT.SetTotalGameTime(std::chrono::milliseconds(100));
			Assert::IsTrue(GT.ElapsedGameTime() == GT.TotalGameTime());
			Assert::IsTrue(GT.ElapsedGameTimeSeconds() == GT.TotalGameTimeSeconds());
			GC.CurrentTime();
			GC.LastTime();
			GC.StartTime();
		}
		TEST_METHOD(CopySemantics)
		{
			CreateConcreteFactory(Entity, Entity)

			World W1;
			World W2(W1);
			World W3;
			W3 = W2;

			Sector *S1 = new Sector(*(W1.CreateSector("S1")));
			Sector *S2 = new Sector();
			*S2 = *S1;
			Assert::IsTrue(S2->GetName() == S1->GetName());

			Entity *E1 = new Entity(*(S1->CreateEntity("Entity", "E1")));
			Entity *E2 = new Entity();
			*E2 = *E1;
			Assert::IsTrue(E1->GetName() == E2->GetName());

			delete S1;
			delete S2;
			delete E1;
			delete E2;

			AbstractFactory<Entity>::ClearFactories();
		}
	};
	_CrtMemState EntitySectorWorldTests::sStartMemState;
}