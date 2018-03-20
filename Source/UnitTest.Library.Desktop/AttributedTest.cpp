#pragma once
#include "pch.h"
#include "CppUnitTest.h"
#define  _CRTDBG_MAP_ALLOC
#include <string>
#include <stdlib.h>
#include <crtdbg.h>
#include <exception>
#include <functional>
#include "Foo.h"
#include "Attributed.h"
#include "AttributedFoo.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(AttributeTest)
	{
	private:
		Attributed A;
		AttributedFoo AF;
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

		TEST_METHOD(RTTITests)
		{
			AttributedFoo A1;
			Assert::IsTrue("AttributedFoo" == A1.TypeName());
			Assert::AreEqual(A1.TypeIdClass(), A1.TypeIdInstance());
			RTTI* PointerToA1 = A1.QueryInterface(A1.TypeIdClass());
			AttributedFoo *A2 = PointerToA1->As<AttributedFoo>();
			Assert::IsTrue(A2->Is(A1.TypeIdInstance()));
			Assert::IsTrue(A2->Is("AttributedFoo"));

			RTTI* R1 = A2;
			Assert::IsFalse(R1->Is("RTTI"));
			Assert::IsFalse(R1->Is(0));
			Assert::IsTrue(R1->QueryInterface(0) == nullptr);

			Assert::IsTrue("RTTI" == R1->RTTI::ToString());
		}

		TEST_METHOD(AttributedInitialization)
		{
			Attributed A1;
		}

		TEST_METHOD(IsAttribute)
		{
			Attributed A1;
			Assert::AreEqual(true, A1.IsAttribute("this"));
			Assert::AreEqual(false, A1.IsAttribute("Hello"));
		}

		TEST_METHOD(AttributedFooInitialization)
		{
			AttributedFoo A1;
			Assert::AreEqual(true, A1.IsPrescribedAttribute("Health"));
			Assert::AreEqual(true, A1.IsPrescribedAttribute("Powers"));
			AttributedFoo A2;
			A2.AppendAuxillaryAttribute("Convoluted") = 42;
			Assert::AreEqual(true, A2.IsAuxillaryAttribute("Convoluted"));

			Assert::AreEqual(100, A1["Health"].Get<int32_t>(0));
			Assert::AreEqual(0.5f, A1["FireRate"].Get<float>(0));
			Assert::AreEqual(std::string("N/A"), A1["Powers"].Get<std::string>(0));
		}

		TEST_METHOD(AddAuxillaryAttributes)
		{
			float ExternalAuxillaryStorage = 8.0f;
			AttributedFoo A1;
			A1.AppendAuxillaryAttribute("ContrivedAttribute1") = 1;
			A1.AppendAuxillaryAttribute("ContrivedAttribute2") = std::string("Contrived String");
			Datum &datum = A1.AppendAuxillaryAttribute("External");
			datum.SetStorage(&ExternalAuxillaryStorage, 1);

			Assert::AreEqual(true, A1.IsAuxillaryAttribute("ContrivedAttribute1"));
			Assert::AreEqual(true, A1.IsAuxillaryAttribute("ContrivedAttribute2"));
			Assert::AreEqual(false, A1.IsPrescribedAttribute("ContrivedAttribute1"));
			Assert::AreEqual(false, A1.IsPrescribedAttribute("ContrivedAttribute3"));
			Assert::AreEqual(ExternalAuxillaryStorage, A1.Find("External")->Get<float>(0));
		}

		TEST_METHOD(GetAttributes)
		{
			float ExternalAuxillaryStorage = 8.0f;
			AttributedFoo A1;
			A1.AppendAuxillaryAttribute("ContrivedAttribute1") = 1;
			A1.AppendAuxillaryAttribute("ContrivedAttribute2") = std::string("Contrived String");
			Datum &datum = A1.AppendAuxillaryAttribute("External");
			datum.SetStorage(&ExternalAuxillaryStorage, 1);

			Assert::AreEqual(true, A1.IsAuxillaryAttribute("ContrivedAttribute1"));
			Assert::AreEqual(true, A1.IsAuxillaryAttribute("ContrivedAttribute2"));
			Assert::AreEqual(false, A1.IsPrescribedAttribute("ContrivedAttribute1"));
			Assert::AreEqual(false, A1.IsPrescribedAttribute("ContrivedAttribute3"));
			Assert::AreEqual(ExternalAuxillaryStorage, A1.Find("External")->Get<float>(0));

			Vector<std::string> AttributesVector = A1.GetAttributes();
			Vector<std::string> PrescribedAttributesVector = A1.GetPrescribedAttributes();
			Vector<std::string> AuxillaryAttributesVector = A1.GetAuxillaryAttributes();

			Assert::AreEqual(24U, AttributesVector.Length());
			Assert::AreEqual(3U, AuxillaryAttributesVector.Length());
			Assert::AreEqual(21U, PrescribedAttributesVector.Length());
		}

		TEST_METHOD(CopyConstructor)
		{
			AttributedFoo A1;
			A1.ContrivedValueChangingFunction();
			AttributedFoo A2(A1);

			Assert::IsTrue(A1 == A2);
		}

		TEST_METHOD(OperatorAssignment)
		{
			AttributedFoo A1;
			A1.ContrivedValueChangingFunction();
			AttributedFoo A2;
			A2 = A1;
		}

		TEST_METHOD(MoveSemantics)
		{
			AttributedFoo A1;
			A1.ContrivedValueChangingFunction();
			AttributedFoo A2(std::move(A1));

			Assert::AreEqual(true, A2.IsPrescribedAttribute("Health"));
			Assert::AreEqual(true, A2.IsPrescribedAttribute("Powers"));

			AttributedFoo A3;
			AttributedFoo A4;
			A4 = std::move(A3);
		}

		TEST_METHOD(ExceptionTesting)
		{
			AttributedFoo A1;
			std::function<void()> Expression = [&A1]() {A1.AppendAuxillaryAttribute("Health"); };
			Assert::ExpectException<std::exception>(Expression);
		}
	};
	_CrtMemState AttributeTest::sStartMemState;
}