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
#include <Shlwapi.h>
#include "JsonParseHelper.h"
#include "JsonParseMaster.h"
#include "JsonParseHelperTable.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ParsingTests)
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

		TEST_METHOD(RTTITests)
		{
			Foo x;
			Foo::FooSharedData A1(x);
			Assert::IsTrue("FooSharedData" == A1.TypeName());
			Assert::AreEqual(A1.TypeIdClass(), A1.TypeIdInstance());
			RTTI* PointerToA1 = A1.QueryInterface(A1.TypeIdClass());
			JsonParseMaster::SharedData *A2 = PointerToA1->As<JsonParseMaster::SharedData>();
			Assert::IsTrue(A2->Is(A1.TypeIdInstance()));
			Assert::IsTrue(A2->Is("FooSharedData"));

			RTTI* R1 = A2;
			Assert::IsFalse(R1->Is("RTTI"));
			Assert::IsFalse(R1->Is(0));
			Assert::IsTrue(R1->QueryInterface(0) == nullptr);

			Assert::IsTrue("RTTI" == R1->RTTI::ToString());
		}

		TEST_METHOD(MasterInitialization)
		{
			JsonParseMaster::SharedData SharedData;
			JsonParseMaster Master1(SharedData);
			std::string JsonString= R"({"book":"Alice in Wonderland","year":1865,"characters":[{"name":"Jabberwock","chapter":1},{"name":"Cheshire Cat","chapter":6},{"name":"Mad Hatter","chapter":7}]})";

			Master1.Parse(JsonString);
			JsonString = R"({"book":"Alice in Wonderland","year":1865,"characters":[{"name":"Jabberwock","chapter":1},{"name":"Cheshire Cat","chapter":6},{"name":"Mad Hatter","chapter":7}]})";
			Master1.Parse(JsonString);

		}

		TEST_METHOD(Parse)
		{
			Scope S1;
			JsonParseHelperTable::TableSharedData SharedScope(S1);
			JsonParseMaster Master1(SharedScope);

			JsonParseHelperTable TableHelper;
			Master1.AddHelper(TableHelper);
			std::string JsonString = R"({"Health":{"type":"Integer","size":1,"value":100},"Speed":{"type":"Float","size":1,"value":10},"PlayerName":{"type":"String","size":1,"value":"Player1"},"Armor":{"type":"Integer","size":3,"value":[100,50,75]},"Skills":{"Invisibility":{"type":"Integer","size":1,"value":3},"Dash":{"type":"Integer","size":1,"value":3}}})";
			Master1.Parse(JsonString);

			Assert::AreEqual(100, S1["Health"].Get<int>(0));
			Assert::AreEqual(10.0f, S1["Speed"].Get<float>(0));
			Assert::AreEqual(std::string("Player1"), S1["PlayerName"].Get<std::string>(0));
			Assert::AreEqual(3U, S1["Armor"].Length());
			Assert::AreEqual(75, S1["Armor"].Get<int>(2));
			Assert::AreEqual(3, S1["Skills"][0]["Dash"].Get<int>(0));
			Assert::AreEqual(3, S1["Skills"][0]["Invisibility"].Get<int>(0));

			std::string JsonString2 = "{\"Position\":{\"type\":\"Vector4\",\"size\":1,\"value\":\"vec4(1.0, 5.3, 2.5, 8.4)\"},\"WVP\":{\"type\":\"Matrix4x4\",\"size\":1,\"value\":\"mat4x4((1.0, 0.0, 0.0, 0.0),(0.0, 1.0, 0.0, 0.0),(0.0, 0.0, 1.0, 0.0),(0.0, 0.0, 0.0, 1.0))\"}}";
			Master1.Parse(JsonString2);
			Assert::IsTrue(glm::vec4(1.0f, 5.3f, 2.5f, 8.4f) == S1["Position"].Get<glm::vec4>(0));
			Assert::IsTrue(glm::mat4x4(1.0f) == S1["WVP"].Get<glm::mat4x4>(0));

			JsonString2 = "{\"Transformation\":{\"type\":\"Vector4\",\"size\":2,\"value\":[\"vec4(1.0, 5.3, 2.5, 8.4)\",\"vec4(1.0, 5.3, 2.5, 8.4)\"]}}";
			Master1.Parse(JsonString2);
			Assert::IsTrue(glm::vec4(1.0f, 5.3f, 2.5f, 8.4f) == S1["Transformation"].Get<glm::vec4>(0));
			Assert::IsTrue(glm::vec4(1.0f, 5.3f, 2.5f, 8.4f) == S1["Transformation"].Get<glm::vec4>(1));

		}

		TEST_METHOD(ParseFromFile)
		{
			std::string FileName = "Scripts/Test.json";

			Scope S1;
			JsonParseHelperTable::TableSharedData SharedScope(S1);
			JsonParseMaster Master1(SharedScope);

			JsonParseHelperTable TableHelper;
			Master1.AddHelper(TableHelper);
			Master1.ParseFromFile(FileName);

			Assert::AreEqual(100, S1["Health"].Get<int32_t>(0));
			Assert::AreEqual(10.0f, S1["Speed"].Get<float>(0));
			Assert::AreEqual(std::string("Player1"), S1["PlayerName"].Get<std::string>(0));
			Assert::AreEqual(3U, S1["Armor"].Length());
			Assert::AreEqual(75, S1["Armor"].Get<int32_t>(2));
			Assert::AreEqual(3, S1["Skills"][0]["Dash"].Get<int32_t>(0));
			Assert::AreEqual(3, S1["Skills"][0]["Invisibility"].Get<int32_t>(0));
			Assert::IsTrue(glm::vec4(1.0f, 5.3f, 2.5f, 8.4f) == S1["Position"].Get<glm::vec4>(0));
			Assert::AreEqual(5, S1["Skills"][0]["ElementalPowers"][0]["Fire"].Get<int32_t>(0));
		}

		TEST_METHOD(ParseFromIfStream)
		{
			std::ifstream InputStream("Scripts/Test.json");
			if (!InputStream.fail())
			{
				Scope S1;
				JsonParseHelperTable::TableSharedData SharedScope(S1);
				JsonParseMaster Master1(SharedScope);

				JsonParseHelperTable TableHelper;
				Master1.AddHelper(TableHelper);
				Master1.Parse(InputStream);

				Assert::AreEqual(100, S1["Health"].Get<int32_t>(0));
				Assert::AreEqual(10.0f, S1["Speed"].Get<float>(0));
				Assert::AreEqual(std::string("Player1"), S1["PlayerName"].Get<std::string>(0));
				Assert::AreEqual(3U, S1["Armor"].Length());
				Assert::AreEqual(75, S1["Armor"].Get<int32_t>(2));
				Assert::AreEqual(3, S1["Skills"][0]["Dash"].Get<int32_t>(0));
				Assert::AreEqual(3, S1["Skills"][0]["Invisibility"].Get<int32_t>(0));
				Assert::IsTrue(glm::vec4(1.0f, 5.3f, 2.5f, 8.4f) == S1["Position"].Get<glm::vec4>(0));
				Assert::AreEqual(5, S1["Skills"][0]["ElementalPowers"][0]["Fire"].Get<int32_t>(0));
			}
		}

		TEST_METHOD(AddingAndRemovingHelpers)
		{
			Foo a;
			Foo::FooSharedData SharedFooData(a);
			FooParser FooParser1;

			JsonParseMaster Master1(SharedFooData);
			Master1.AddHelper(FooParser1);
			std::string JsonString = R"({"TestMember":1,"TestMember2":2})";
			Master1.Parse(JsonString);

			Assert::IsTrue(a == Foo(1,2));

			Master1.RemoveHelper(FooParser1);
		}

		TEST_METHOD(Clone)
		{
			Foo a;
			Foo::FooSharedData SharedFooData(a);
			FooParser FooParser1;

			JsonParseMaster Master1;
			Master1.SetSharedData(SharedFooData);

			Assert::IsTrue(&Master1 == SharedFooData.GetJsonParseMaster());

			Master1.AddHelper(FooParser1);

			Assert::IsTrue(Master1.GetSharedData() == SharedFooData.As<JsonParseMaster::SharedData>());

			JsonParseMaster *Master1Clone = Master1.Clone();

			std::string JsonString = R"({"TestMember":1,"TestMember2":2})";
			Master1Clone->Parse(JsonString);

			Assert::AreEqual(true, Master1Clone->IsParseMasterClone());
			Assert::IsTrue( *Master1Clone->GetSharedData()->As<Foo::FooSharedData>()->GetData() == Foo(1, 2));
			
			Scope S1;
			JsonParseHelperTable::TableSharedData TableData(S1);
			JsonParseHelperTable TableHelper;

			TableData.SetSharedScope(S1);

			JsonParseMaster Master2(TableData);
			Master2.AddHelper(TableHelper);

			JsonParseMaster *Master2Clone = Master2.Clone();

			delete Master2Clone->GetSharedData()->As<JsonParseHelperTable::TableSharedData>()->GetSharedScope();
			delete Master2Clone;
;			delete Master1Clone->GetSharedData()->As<Foo::FooSharedData>()->GetData();
			delete Master1Clone;
			
		}

		TEST_METHOD(ExceptionTesting)
		{
			std::string FileName = "QQ.json";

			Foo a;
			Foo::FooSharedData SharedFooData(a);
			FooParser FooParser1;

			JsonParseMaster Master1(SharedFooData);
			Master1.AddHelper(FooParser1);

			std::function<void()> Expression = [&Master1, &FileName]() {Master1.ParseFromFile(FileName); };
			Assert::ExpectException<std::exception>(Expression);

			JsonParseMaster::SharedData SharedData;
			JsonParseMaster Master2(SharedData);
			JsonParseMaster *Master2Clone = Master2.Clone();

			Expression = [&Master2Clone, &FooParser1]() {Master2Clone->AddHelper(FooParser1); };
			Assert::ExpectException<std::exception>(Expression);

			delete Master2Clone;
		}

		TEST_METHOD(Reset)
		{
			std::string FileName = "Scripts/Test.json";
			JsonParseMaster::SharedData SharedData;
			JsonParseMaster Master1(SharedData);
			Master1.ParseFromFile(FileName);

			Assert::AreEqual(FileName, Master1.GetFileName());
			Master1.Reset();
			Assert::AreEqual("", Master1.GetFileName().c_str());

			FooParser F1;

			JsonParseMaster Master2(SharedData);
			Master2.AddHelper(F1);
			JsonParseMaster *Master2Clone = Master2.Clone();
			Master2Clone->Reset();

			delete Master2Clone;
		}
	};
	_CrtMemState ParsingTests::sStartMemState;
}