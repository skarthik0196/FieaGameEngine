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
#include "AbstractFactory.h"
#include "ConcreteFactory.h"
#include "Foo.h"
#include "Scope.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;



namespace UnitTestLibraryDesktop
{
	TEST_CLASS(FactoryTests)
	{
	private:

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
			CreateConcreteFactory(Scope, RTTI)
			CreateConcreteFactory(Foo, RTTI)
			auto It = AbstractFactory<RTTI>::begin();

			AbstractFactory<RTTI>::ClearFactories();
		}

		TEST_METHOD(ScopeCreation)
		{
			CreateConcreteFactory(Scope, RTTI)
			CreateConcreteFactory(Foo, RTTI)

			Scope *S1 = AbstractFactory<RTTI>::Create("Scope")->As<Scope>();
			S1->Append("Health") = 100;
			Assert::IsTrue((*S1)["Health"] == 100);

			AbstractFactory<RTTI>::ClearFactories();
			delete S1;
			
		}

		TEST_METHOD(FooCreation)
		{
			CreateConcreteFactory(Scope, RTTI)
			CreateConcreteFactory(Foo, RTTI)

			Foo *f1 = AbstractFactory<RTTI>::Create("Foo")->As<Foo>();
			Assert::IsTrue(*f1 == Foo(0, 0));

			delete f1;
			AbstractFactory<RTTI>::ClearFactories();
		}

		TEST_METHOD(Find)
		{
			CreateConcreteFactory(Scope, RTTI)
			CreateConcreteFactory(Foo, RTTI)

			Scope *S1 = AbstractFactory<RTTI>::Create("Scope")->As<Scope>();
			S1->Append("Health") = 100;
			Assert::IsTrue((*S1)["Health"] == 100);

			std::string Name = "Scope";
			auto CFactory = AbstractFactory<RTTI>::Find(Name);
			Assert::AreEqual(CFactory->ClassName(), Name);

			AbstractFactory<RTTI>::ClearFactories();
			delete S1;
		}

		TEST_METHOD(BeginEnd)
		{
			CreateConcreteFactory(Scope, RTTI);
			CreateConcreteFactory(Foo, RTTI);

			auto It = AbstractFactory<RTTI>::begin();
			Assert::AreEqual(It->first, std::string("Scope"));
			++It;
			Assert::AreEqual(It->first, std::string("Foo"));
			++It;
			Assert::IsTrue(It == AbstractFactory<RTTI>::end());

			AbstractFactory<RTTI>::ClearFactories();
		}

		TEST_METHOD(BadCreation)
		{
			CreateConcreteFactory(Scope, RTTI);
			CreateConcreteFactory(Foo, RTTI);

			RTTI* CreatedObject= AbstractFactory<RTTI>::Create("None");
			Assert::IsNull(CreatedObject);

			AbstractFactory<RTTI>::ClearFactories();
		}

		TEST_METHOD(Remove)
		{
			CreateConcreteFactory(Scope, RTTI);
			CreateConcreteFactory(Foo, RTTI);

			AbstractFactory<RTTI>* A = AbstractFactory<RTTI>::Find("Scope");
			AbstractFactory<RTTI>::RemoveFactory(*A);

			AbstractFactory<RTTI>::AddFactory("Scope", *A);
			AbstractFactory<RTTI>::RemoveAndDeleteFactory("Scope");
			AbstractFactory<RTTI>::RemoveAndDeleteFactory("Foo");

			//AbstractFactory<RTTI>::ClearFactories();
		}
	};
	_CrtMemState FactoryTests::sStartMemState;
}