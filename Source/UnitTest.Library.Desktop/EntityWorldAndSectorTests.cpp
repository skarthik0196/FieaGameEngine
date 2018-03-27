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
			CreateConcreteFactory(Entity, RTTI)

			World W1("W1");
			Assert::AreEqual(std::string("W1"), W1.GetName());

			Sector* S1 = W1.CreateSector("S1");
			Assert::AreEqual((*S1)["Name"].Get<std::string>(0), S1->GetName());
			Assert::AreEqual(std::string("S1"), S1->GetName());

			Entity* E1 = S1->CreateEntity("Entity", "E1");
			Assert::AreEqual(std::string("E1"), E1->GetName());

			AbstractFactory<RTTI>::ClearFactories();
		}

		TEST_METHOD(Update)
		{
			CreateConcreteFactory(Entity, RTTI)

			World W1("World1");
			Sector *S1 = W1.CreateSector("S1");
			Sector *S2 = W1.CreateSector("S2");

			S1->CreateEntity("Entity", "E1");
			S1->CreateEntity("Entity", "E2");

			S2->CreateEntity("Entity", "E3");

			W1.Update();
			
			Assert::IsTrue(W1.GetWorldState().CurrentEntity == S2->GetEntities().Get<Scope*>(0));

			AbstractFactory<RTTI>::ClearFactories();
		}

		TEST_METHOD(GetName)
		{
			CreateConcreteFactory(Entity, RTTI)

			World W1("W1");
			Assert::AreEqual(std::string("W1"), W1.GetName());

			Sector* S1 = W1.CreateSector("S1");
			Assert::AreEqual((*S1)["Name"].Get<std::string>(0), S1->GetName());
			Assert::AreEqual(std::string("S1"), S1->GetName());

			Entity* E1 = S1->CreateEntity("Entity", "E1");
			Assert::AreEqual(std::string("E1"), E1->GetName());

			AbstractFactory<RTTI>::ClearFactories();
		}

		TEST_METHOD(GetSectorGetWorld)
		{
			CreateConcreteFactory(Entity, RTTI)

			World W1("W1");
			Assert::AreEqual(std::string("W1"), W1.GetName());

			Sector* S1 = W1.CreateSector("S1");
			Assert::AreEqual((*S1)["Name"].Get<std::string>(0), S1->GetName());
			Assert::AreEqual(std::string("S1"), S1->GetName());
			Assert::IsTrue(S1->GetWorld() == &W1);

			Entity* E1 = S1->CreateEntity("Entity", "E1");
			Assert::AreEqual(std::string("E1"), E1->GetName());
			Assert::IsTrue(E1->GetSector() == S1);

			AbstractFactory<RTTI>::ClearFactories();
		}

		TEST_METHOD(Entities)
		{
			CreateConcreteFactory(Entity, RTTI)

			World W1("World1");
			Sector *S1 = W1.CreateSector("S1");

			S1->CreateEntity("Entity", "E1");
			Entity* E2 = S1->CreateEntity("Entity", "E2");
			S1->CreateEntity("Entity", "E3");

			Datum& Entities = S1->GetEntities();
			Assert::AreEqual(3U, Entities.Length());
			Assert::IsTrue(Entities.Get<Scope*>(1) == E2);

			AbstractFactory<RTTI>::ClearFactories();
		}

		TEST_METHOD(Sectors)
		{
			CreateConcreteFactory(Entity, RTTI)

			World W1("World1");
			Sector *S1 = W1.CreateSector("S1");
			W1.CreateSector("S2");
			S1->CreateEntity("Entity", "E1");
			S1->CreateEntity("Entity", "E2");
			S1->CreateEntity("Entity", "E3");

			Datum& Sectors = W1.GetSectors();
			Assert::AreEqual(2U, Sectors.Length());
			Assert::IsTrue(S1 == Sectors.Get<Scope*>(0));

			AbstractFactory<RTTI>::ClearFactories();
		}

		TEST_METHOD(Parsing)
		{
			CreateConcreteFactory(Entity, RTTI)

			std::string FileName = "Scripts/EntityParseTest.json";

			World W1;
			Sector *S1 = W1.CreateSector("S1");
			JsonParseHelperTable::TableSharedData SharedScope(*S1);
			JsonParseMaster Master1(SharedScope);

			JsonParseHelperTable TableHelper;
			Master1.AddHelper(TableHelper);
			Master1.ParseFromFile(FileName);

			Assert::AreEqual((*S1)["Weather"].Get<std::string>(0), std::string("Sunny"));
			Assert::AreEqual(1U, S1->GetEntities().Length());
			Entity *E1 = (*S1)["Entities"].Get<Scope*>(0)->As<Entity>();
			Assert::AreEqual((*E1)["Invisibility"].Get<int32_t>(0), 3);
			Assert::AreEqual((*E1)["Dash"].Get<int32_t>(0), 3);

			AbstractFactory<RTTI>::ClearFactories();
		}
	};
	_CrtMemState EntitySectorWorldTests::sStartMemState;
}