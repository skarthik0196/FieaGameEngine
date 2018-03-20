#pragma once
#include "pch.h"
#define _CRTDBG_MAP_ALLOC
#include<stdlib.h>
#include<crtdbg.h>
#include "CppUnitTest.h"
#include<string>
#include<exception>
#include<functional>
#include"Foo.h"
#include"Datum.h"
#include"RTTI.h"
#include"Scope.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ScopeTest)
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

		TEST_METHOD(ScopeInitialization)
		{
			Scope S1;
			S1.Append("Health") = 100;
		}
		
		TEST_METHOD(RTTITest)
		{
			RTTI *a = new Scope();
			Assert::AreEqual(std::string("Scope"), a->As<Scope>()->ToString());
			delete a;

		}
		TEST_METHOD(Append)
		{
			Scope S1;
			S1.Append("Health") = 100;
			Assert::IsTrue(S1[0] == 100);
			S1.Append("Position") = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			Assert::IsTrue(S1[1] == glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		}

		TEST_METHOD(AppendScope)
		{
			Scope S1;

			S1.Append("Health") = 100;
			Assert::IsTrue(S1[0] == 100);
			S1.Append("Position") = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			Assert::IsTrue(S1[1] == glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			
			S1.AppendScope("Player Details").Append("Player Number") = 1;
			S1["Player Details"][0].Append("Player Color") = std::string("Red");
			
			Assert::IsTrue(S1["Player Details"].Get<Scope*>(0)->GetParent() == &S1);
		}

		TEST_METHOD(IndexDereferencing)
		{
			Scope S1;
			S1.Append("Health") = 100;
			Assert::IsTrue(S1[0] == 100);
			S1.Append("Position") = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			Assert::IsTrue(S1[1] == glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

			Datum d1;
		}

		TEST_METHOD(Find)
		{
			Scope S1;
			S1.Append("Health") = 100;
			Assert::IsTrue(S1[0] == 100);
			S1.Append("Position") = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			Assert::IsTrue(S1[1] == glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			Datum *D = S1.Find("Armor");
			Assert::IsTrue(D == nullptr);
			D = S1.Find("Position");
			Assert::IsTrue(*D == glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		}

		TEST_METHOD(FindName)
		{
			Scope S1;
			S1.Append("Health") = 100;
			Assert::IsTrue(S1[0] == 100);
			S1.Append("Position") = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			Assert::IsTrue(S1[1] == glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

			S1.AppendScope("Player Details").Append("Player Number") = 1;
			S1["Player Details"][0].Append("Player Color") = std::string("Red");

			Scope *S2 = &(S1.AppendScope("Contrived Info"));

			Assert::AreEqual(std::string("Contrived Info"), S1.FindName(*S2));
		}

		TEST_METHOD(Search)
		{
			Scope S1;

			S1.Append("Health") = 100;
			Assert::IsTrue(S1[0] == 100);
			S1.Append("Position") = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			Assert::IsTrue(S1[1] == glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

			S1.AppendScope("Player Details").Append("Player Number") = 1;
			S1["Player Details"][0].Append("Player Color") = std::string("Red");
			S1["Player Details"][0].AppendScope("Convoluted Information").Append("Contrived Info") = std::string("Why?");

			Scope *a, *b;
			Scope **scopeLocation = &a;
			Datum *d;
			d = S1["Player Details"][0]["Convoluted Information"][0].Search("Health" , scopeLocation);
			b = &(S1["Player Details"][0]);
			Assert::IsTrue(&S1 == *(scopeLocation));

			Scope *c = a;
			d = S1.Search("Existence", &c);
			Assert::IsNull(c);
		}

		TEST_METHOD(Adopt)
		{
			Scope *S1 = new Scope();
			S1->Append("Health") = 100;

			Scope *S2 = new Scope();
			S2->Append("Contrived Value") = 0.01f;

			S1->AppendScope("Attributes").Append("Speed") = 10;
			(*S1)["Attributes"][0].Append("Dexterity") = 16;

			S2->AppendScope("Powers").Append("Dash") = 1;

			S1->Adopt((*S2)["Powers"][0], "Skills");
			Assert::IsTrue((*S1)["Skills"][0]["Dash"] == 1);

			delete S1;
			delete S2;
		}

		TEST_METHOD(OperatorEquality)
		{
			Scope *S1 = new Scope();
			S1->Append("Health") = 100;

			Scope *S2 = new Scope();
			S2->Append("Health") = 100;

			S1->AppendScope("Attributes").Append("Speed") = 10;

			S2->AppendScope("Attributes").Append("Speed") = 10;

			Assert::IsTrue(*S1 == *S2);
			
			delete S1;
			delete S2;

		}

		TEST_METHOD(OperatorInequality)
		{
			Scope *S1 = new Scope();
			S1->Append("Health") = 100;

			Scope *S2 = new Scope();
			S2->Append("Health") = 100;

			S1->AppendScope("Attributes").Append("Speed") = 10;

			S2->AppendScope("Attributes").Append("Speed") = 100;

			Assert::IsTrue(*S1 != *S2);

			delete S1;
			delete S2;
		}

		TEST_METHOD(Equals)
		{

			Scope *S1 = new Scope();
			S1->Append("Health") = 100;

			Scope *S2 = new Scope();
			S2->Append("Health") = 100;

			S1->AppendScope("Attributes").Append("Speed") = 10;

			S2->AppendScope("Attributes").Append("Speed") = 10;

			Assert::IsTrue(S1->Equals(S2));

			delete S1;
			delete S2;
		}

		TEST_METHOD(ChildDestruction)
		{
			Scope *S1 = new Scope();

			S1->Append("Health") = 100;
			Assert::IsTrue((*S1)[0] == 100);
			S1->Append("Position") = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			Assert::IsTrue((*S1)[1] == glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

			S1->AppendScope("Player Details").Append("Player Number") = 1;
			S1->AppendScope("Player Details").Append("Player Number") = 1;
			(*S1)["Player Details"][0].Append("Player Color") = std::string("Red");

			Scope *S2 = &(S1->AppendScope("Contrived Scope"));
			Scope *S3 = &(S1->AppendScope("Contrived Scope"));
			S2;
			delete S3;
			//delete S2;
			delete S1;
		}

		TEST_METHOD(ExceptionTesting)
		{
			Scope *S1 = new Scope();

			S1->Append("Health") = 100;
			Assert::IsTrue((*S1)[0] == 100);
			std::function<void()> Expression = [&S1]() {S1->AppendScope("Health");};
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&S1]() {S1->Adopt(*S1, "ContrivedTestCase"); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&S1]() {S1->Orphan(S1); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&S1]() {S1->Append(std::string("")); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&S1]() {S1->Equals(nullptr); };
			Assert::ExpectException<std::exception>(Expression);

			delete S1;
			
		}

		TEST_METHOD(CopyConstructor)
		{
			Scope *S1 = new Scope();

			S1->Append("Health") = 100;
			Assert::IsTrue((*S1)[0] == 100);
			S1->Append("Position") = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			Assert::IsTrue((*S1)[1] == glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

			S1->AppendScope("Player Details").Append("Player Number") = 1;

			Scope *S2 = new Scope(*S1);

			Scope *S3 = new Scope(*((*S1)["Player Details"].Get<Scope*>(0)));

			Assert::IsTrue(*S1 == *S2);

			Assert::IsTrue(*((*S1)["Player Details"].Get<Scope*>(0)) == *S3);

			Assert::IsFalse(*S1 == *S3);

			delete S1;
			delete S2;
			delete S3;
		}

		TEST_METHOD(AssignmentOperator)
		{
			Scope *S1 = new Scope();

			S1->Append("Health") = 100;
			Assert::IsTrue((*S1)[0] == 100);
			S1->Append("Position") = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			Assert::IsTrue((*S1)[1] == glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

			S1->AppendScope("Player Details").Append("Player Number") = 1;

			Scope *S2 = new Scope();

			*S2 = *S1;
			Assert::IsTrue(*S1 == *S2);

			Scope *S3 = new Scope();
			*S3 = (*((*S1)["Player Details"].Get<Scope*>(0)));

			Assert::IsTrue(*((*S1)["Player Details"].Get<Scope*>(0)) == *S3);

			delete S1;
			delete S2;
			delete S3;
		}

		TEST_METHOD(MoveSemantics)
		{
			Scope S1;

			S1.Append("Health") = 100;
			Assert::IsTrue(S1[0] == 100);
			S1.Append("Position") = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			Assert::IsTrue(S1[1] == glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

			S1.AppendScope("Player Details").Append("Player Number") = 1;

			Scope S2 = std::move(S1);

			Assert::IsTrue(S2[0] == 100);
			Assert::IsTrue(S2[1] == glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

			Scope S4;
			S4 = std::move(S2);

			Assert::IsTrue(S4[0] == 100);
			Assert::IsTrue(S4[1] == glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

			Scope *S3;
			S3 = S4["Player Details"].Get<Scope*>(0);
			Scope *S5 = new Scope(std::move(*S3));
			Scope *S6 = new Scope();
			*S6 = std::move(*S5);
			delete S3;
			delete S5;
		}
	};


	_CrtMemState ScopeTest::sStartMemState;
}