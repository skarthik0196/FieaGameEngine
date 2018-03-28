#pragma once
#include "pch.h"
#include "CppUnitTest.h"
#define _CRTDBG_MAP_ALLOC
#include<string>
#include<stdlib.h>
#include<crtdbg.h>
#include<exception>
#include<functional>
#include "Foo.h"
#include"Datum.h"
#include"RTTI.h"
#include"Scope.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<UnitTestLibraryDesktop::Foo>(const UnitTestLibraryDesktop::Foo& T)
	{
		RETURN_WIDE_STRING(T.GetData());
	}

	template<>
	std::wstring ToString<UnitTestLibraryDesktop::Foo>(UnitTestLibraryDesktop::Foo* T)
	{
		RETURN_WIDE_STRING(T->GetData());
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DatumTest)
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

		TEST_METHOD(DatumInitialization)
		{
			Datum d1;

			int32_t a[5] = { 3,5,6,26,3 };

			Datum d2;
			d2.SetStorage(a, 5);
			Assert::IsTrue(Datum::DatumType::Integer == d2.GetType());
		}

		TEST_METHOD(ScalarInitialization)
		{
			Datum d1 = 4;
			Datum d2 = std::string("Hello World!");
			Datum d3 = 4.5f;
			Datum d4 = glm::vec4(0.5f, 5.6f, 7.9f, 8.3f);
			Datum d5 = glm::mat4x4(glm::mat4x4(1.0f));
			Foo a(5, 6);
			Datum d6 = &a;

			Assert::AreEqual(4, d1.Get<int32_t>());
			Assert::AreEqual(std::string("Hello World!"), d2.Get<std::string>());
			Assert::AreEqual(4.5f, d3.Get<float>());
			Assert::IsTrue(glm::vec4(0.5f, 5.6f, 7.9f, 8.3f) == d4.Get<glm::vec4>());
			Assert::IsTrue(glm::mat4x4(glm::mat4x4(1.0f)) == d5.Get<glm::mat4x4>());
			Assert::AreEqual(a, *dynamic_cast<Foo*>((d6.Get<RTTI*>(0))));
		}

		TEST_METHOD(ScalarAssignment)
		{
			Datum d1;
			d1 = 4;
			d1 = 4;
			Datum d2;
			d2 = std::string("Hello World!");
			d2 = std::string("Hello World!");
			Datum d3;
			d3 = 4.5f;
			d3 = 4.5f;
			Datum d4;
			d4 = glm::vec4(0.5f, 5.6f, 7.9f, 8.3f);
			d4 = glm::vec4(0.5f, 5.6f, 7.9f, 8.3f);
			Datum d5;
			d5 = glm::mat4x4(glm::mat4x4(1.0f));
			d5 = glm::mat4x4(glm::mat4x4(1.0f));
			Foo a(5, 6);
			Datum d6;
			d6 = &a;
			d6 = &a;

			Assert::AreEqual(4, d1.Get<int32_t>());
			Assert::AreEqual(std::string("Hello World!"), d2.Get<std::string>());
			Assert::AreEqual(4.5f, d3.Get<float>());
			Assert::IsTrue(glm::vec4(0.5f, 5.6f, 7.9f, 8.3f) == d4.Get<glm::vec4>());
			Assert::IsTrue(glm::mat4x4(glm::mat4x4(1.0f)) == d5.Get<glm::mat4x4>());
			Assert::AreEqual(a, *dynamic_cast<Foo*>((d6.Get<RTTI*>(0))));
		}

		TEST_METHOD(PushBack)
		{
			int a[] = { 3,6,7,32,6 };
			Datum d1;
			for (int i = 0; i < 5; i++)
			{
				d1.PushBack(a[i]);
				Assert::AreEqual(a[i], d1.Get<int32_t>(i));
			}

			float b[] = { 5.6f,5.1f,6.5f,8.5f,7.7f };
			Datum d2;
			for (int i = 0; i < 5; i++)
			{
				d2.PushBack(b[i]);
				Assert::AreEqual(b[i], d2.Get<float>(i));
			}

			std::string c[] = { "Hello", "World", "Greetings", "Cool", "C++" };
			Datum d3;
			for (int i = 0; i < 5; i++)
			{
				d3.PushBack(c[i]);
				Assert::AreEqual(c[i], d3.Get<std::string>(i));
			}

			glm::vec4 d[] = { { 4.5f, 6.3f, 2.4f, 1.0f },{0.5f,5.6f,8.54f, 5.6f}, {5.6f,9.6f,1.2f,6.2f} };
			Datum d4;
			for (int i = 0; i < 3; i++)
			{
				d4.PushBack(d[i]);
				Assert::IsTrue(d[i] == d4.Get<glm::vec4>(i));
			}

			glm::mat4x4 e[] = { {glm::mat4x4(1.0f)},{ glm::mat4x4(10.0f) },{ glm::mat4x4(5.0f) } };
			Datum d5;
			for (int i = 0; i < 3; i++)
			{
				d5.PushBack(e[i]);
				Assert::IsTrue(e[i] == d5.Get<glm::mat4x4>(i));
			}

			Foo f(3, 5);
			Datum d6;
			d6.PushBack(&f);
			Assert::AreEqual(f, *dynamic_cast<Foo*>((d6.Get<RTTI*>(0))));

			Scope *newScope = new Scope();
			Datum d7 = newScope;
			d7 = newScope;

			delete newScope;
		}

		TEST_METHOD(SetStorage)
		{
			int a[] = { 3,6,7,32,6 };
			Datum d1;
			for (int i = 0; i < 5; i++)
			{
				d1.PushBack(a[i]);
				Assert::AreEqual(a[i], d1.Get<int32_t>(i));
			}

			float b[] = { 5.6f,5.1f,6.5f,8.5f,7.7f };
			Datum d2;
			for (int i = 0; i < 5; i++)
			{
				d2.PushBack(b[i]);
				Assert::AreEqual(b[i], d2.Get<float>(i));
			}

			std::string c[] = { "Hello", "World", "Greetings", "Cool", "C++" };
			Datum d3;
			for (int i = 0; i < 5; i++)
			{
				d3.PushBack(c[i]);
				Assert::AreEqual(c[i], d3.Get<std::string>(i));
			}

			glm::vec4 d[] = { { 4.5f, 6.3f, 2.4f, 1.0f },{ 0.5f,5.6f,8.54f, 5.6f },{ 5.6f,9.6f,1.2f,6.2f } };
			Datum d4;
			for (int i = 0; i < 3; i++)
			{
				d4.PushBack(d[i]);
				Assert::IsTrue(d[i] == d4.Get<glm::vec4>(i));
			}

			glm::mat4x4 e[] = { { glm::mat4x4(1.0f) },{ glm::mat4x4(10.0f) },{ glm::mat4x4(5.0f) } };
			Datum d5;
			for (int i = 0; i < 3; i++)
			{
				d5.PushBack(e[i]);
				Assert::IsTrue(e[i] == d5.Get<glm::mat4x4>(i));
			}

			Foo f(3, 5);
			Datum d6;
			d6.PushBack(&f);
			Assert::AreEqual(f, *dynamic_cast<Foo*>((d6.Get<RTTI*>(0))));
			RTTI* p = &f;

			d1.SetStorage(a, 5);
			d2.SetStorage(b, 5);
			d3.SetStorage(c, 5);
			d4.SetStorage(d, 3);
			d5.SetStorage(e, 3);
			d6.SetStorage(&p, 1);
		}

		TEST_METHOD(SetAndGet)
		{
			Datum d1, d2, d3, d4, d5, d6;
			Foo a(3, 6);

			int32_t Value = 4;
			glm::vec4 Vec4Value = { 4.5f, 6.3f, 2.4f, 1.0f };

			d1.PushBack(1);
			d1.Set(Value);
			Assert::AreEqual(Value, d1.Get<int32_t>(0U));
			d1.PushBack(1);
			d1.Set(Value);

			d2.PushBack(glm::vec4(5.3f, 2.6f, 2.2f, 1.6f));
			d2.Set(Vec4Value);
			Assert::IsTrue(Vec4Value == d2.Get<glm::vec4>(0));
			d2.PushBack(glm::vec4(5.3f, 2.6f, 2.2f, 1.6f));
			d2.Set(Vec4Value);

			d3.PushBack(2.5f);
			d3.Set(4.7f);
			Assert::AreEqual(4.7f, d3.Get<float>(0U));
			d3.PushBack(2.5f);
			d3.Set(4.7f);

			d4.PushBack(glm::mat4x4(glm::mat4x4(2.0f)));
			d4.Set(glm::mat4x4(glm::mat4x4(1.0)));
			Assert::IsTrue(glm::mat4x4(1.0) == d4.Get<glm::mat4x4>(0));
			d4.PushBack(glm::mat4x4(glm::mat4x4(2.0f)));
			d4.Set(glm::mat4x4(glm::mat4x4(1.0)));

			Foo b(4, 5);
			d5.PushBack(&b);
			d5.Set(&a);
			Assert::AreEqual(&a, dynamic_cast<Foo*>(d5.Get<RTTI*>()));
			d5.PushBack(&b);
			d5.Set(&a);

			d6.PushBack(std::string("Hello World"));
			d6.Set(std::string("Hello, Hollow World"));
			Assert::AreEqual(std::string("Hello, Hollow World"), d6.Get<std::string>());
			d6.PushBack(std::string("Hello World"));
			d6.Set(std::string("Hello, Hollow World"));

			int x[] = { 3,6,7,32,6 };
			Datum d7;
			d7.SetStorage(x, ARRAYSIZE(x));

			for (int i = 0; i < ARRAYSIZE(x); ++i)
			{
				Assert::AreEqual(x[i], d7.Get<int32_t>(i));
			}
			d7.Set(1, 4);
			Assert::AreEqual(1, d7.Get<int32_t>(4));

			const Datum &cd1 = d1;
			const Datum &cd2 = d2;
			const Datum &cd3 = d3;
			const Datum &cd4 = d4;
			const Datum &cd5 = d5;
			const Datum &cd6 = d6;

			Assert::AreEqual(Value, cd1.Get<int32_t>(0U));
			Assert::IsTrue(Vec4Value == cd2.Get<glm::vec4>(0));
			Assert::IsTrue(glm::mat4x4(1.0) == cd4.Get<glm::mat4x4>(0));
			Assert::AreEqual(std::string("Hello, Hollow World"), cd6.Get<std::string>());
			Assert::AreEqual(&a, dynamic_cast<Foo*>(cd5.Get<RTTI*>()));
			Assert::AreEqual(4.7f, cd3.Get<float>(0U));
		}

		TEST_METHOD(CopyConstructor)
		{
			int a[] = { 3,6,7,32,6 };
			Datum d1;
			for (int i = 0; i < 5; i++)
			{
				d1.PushBack(a[i]);
				Assert::AreEqual(a[i], d1.Get<int32_t>(i));
			}
			Datum dd1(d1);
			Datum exd1;
			exd1.SetStorage(a, 5);

			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(d1.Get<int32_t>(i), dd1.Get<int32_t>(i));
			}
			Datum exd2(exd1);

			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(exd1.Get<int32_t>(i), exd2.Get<int32_t>(i));
			}

			float b[] = { 5.6f,5.1f,6.5f,8.5f,7.7f };
			Datum d2;
			for (int i = 0; i < 5; i++)
			{
				d2.PushBack(b[i]);
				Assert::AreEqual(b[i], d2.Get<float>(i));
			}
			Datum dd2(d2);
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(d2.Get<float>(i), dd2.Get<float>(i));
			}

			std::string c[] = { "Hello", "World", "Greetings", "Cool", "C++" };
			Datum d3;
			for (int i = 0; i < 5; i++)
			{
				d3.PushBack(c[i]);
				Assert::AreEqual(c[i], d3.Get<std::string>(i));
			}
			Datum dd3(d3);
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(d3.Get<std::string>(i), dd3.Get<std::string>(i));
			}

			glm::vec4 d[] = { { 4.5f, 6.3f, 2.4f, 1.0f },{ 0.5f,5.6f,8.54f, 5.6f },{ 5.6f,9.6f,1.2f,6.2f } };
			Datum d4;
			for (int i = 0; i < 3; i++)
			{
				d4.PushBack(d[i]);
				Assert::IsTrue(d[i] == d4.Get<glm::vec4>(i));
			}
			Datum dd4(d4);
			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(d4.Get<glm::vec4>(i) == dd4.Get<glm::vec4>(i));
			}

			glm::mat4x4 e[] = { { glm::mat4x4(1.0f) },{ glm::mat4x4(10.0f) },{ glm::mat4x4(5.0f) } };
			Datum d5;
			for (int i = 0; i < 3; i++)
			{
				d5.PushBack(e[i]);
				Assert::IsTrue(e[i] == d5.Get<glm::mat4x4>(i));
			}
			Datum dd5(d5);
			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(d5.Get<glm::mat4x4>(i) == dd5.Get<glm::mat4x4>(i));
			}

			Foo f(3, 5);
			Datum d6;
			d6.PushBack(&f);
			Assert::AreEqual(f, *dynamic_cast<Foo*>((d6.Get<RTTI*>(0))));

			Datum dd6(d6);
			Assert::AreEqual(*dynamic_cast<Foo*>((d6.Get<RTTI*>(0))), *dynamic_cast<Foo*>((dd6.Get<RTTI*>(0))));
		}

		TEST_METHOD(OperatorAssignent)
		{
			int a[] = { 3,6,7,32,6 };
			int aa[5] = { 2,6,7,3,8 };
			Datum d1, exd1;
			for (int i = 0; i < 5; i++)
			{
				d1.PushBack(a[i]);
				Assert::AreEqual(a[i], d1.Get<int32_t>(i));
			}
			Assert::IsTrue(d1 == d1);
			Datum dd1;
			dd1=d1;
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(d1.Get<int32_t>(i), dd1.Get<int32_t>(i));
			}
			exd1.SetStorage(aa, 5);
			dd1 = exd1;
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(aa[i], dd1.Get<int32_t>(i));
			}

			float b[] = { 5.6f,5.1f,6.5f,8.5f,7.7f };
			Datum d2;
			for (int i = 0; i < 5; i++)
			{
				d2.PushBack(b[i]);
				Assert::AreEqual(b[i], d2.Get<float>(i));
			}
			Datum dd2;
			dd2 = d2;
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(d2.Get<float>(i), dd2.Get<float>(i));
			}
			exd1.SetStorage(b, 5);
			dd2 = exd1;
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(exd1.Get<float>(i), dd2.Get<float>(i));
			}

			std::string c[] = { "Hello", "World", "Greetings", "Cool", "C++" };
			Datum d3;
			for (int i = 0; i < 5; i++)
			{
				d3.PushBack(c[i]);
				Assert::AreEqual(c[i], d3.Get<std::string>(i));
			}
			Datum dd3;
			dd3=d3;
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(d3.Get<std::string>(i), dd3.Get<std::string>(i));
			}
			exd1.SetStorage(c, 5);
			dd3 = exd1;
			dd2 = exd1;
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(exd1.Get<std::string>(i), dd3.Get<std::string>(i));
				Assert::AreEqual(exd1.Get<std::string>(i), dd2.Get<std::string>(i));
			}


			glm::vec4 d[] = { { 4.5f, 6.3f, 2.4f, 1.0f },{ 0.5f,5.6f,8.54f, 5.6f },{ 5.6f,9.6f,1.2f,6.2f } };
			Datum d4;
			for (int i = 0; i < 3; i++)
			{
				d4.PushBack(d[i]);
				Assert::IsTrue(d[i] == d4.Get<glm::vec4>(i));
			}
			Datum dd4;
			dd4 = d4;
			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(d4.Get<glm::vec4>(i) == dd4.Get<glm::vec4>(i));
			}
			exd1.SetStorage(d, 5);
			dd4 = exd1;
			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(exd1.Get<glm::vec4>(i) == dd4.Get<glm::vec4>(i));
			}

			glm::mat4x4 e[] = { { glm::mat4x4(1.0f) },{ glm::mat4x4(10.0f) },{ glm::mat4x4(5.0f) } };
			Datum d5;
			for (int i = 0; i < 3; i++)
			{
				d5.PushBack(e[i]);
				Assert::IsTrue(e[i] == d5.Get<glm::mat4x4>(i));
			}
			Datum dd5;
			dd5 = d5;
			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(d5.Get<glm::mat4x4>(i) == dd5.Get<glm::mat4x4>(i));
			}
			exd1.SetStorage(e, 5);
			dd5 = exd1;
			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(exd1.Get<glm::mat4x4>(i) == dd5.Get<glm::mat4x4>(i));
			}

			Foo f(3, 5), g(4, 8);
			Datum d6;
			d6.PushBack(&f);
			d6.PushBack(&g);
			Assert::AreEqual(f, *dynamic_cast<Foo*>((d6.Get<RTTI*>(0))));

			Datum dd6;
			dd6 = d6;
			Assert::AreEqual(*dynamic_cast<Foo*>((d6.Get<RTTI*>(0))), *dynamic_cast<Foo*>((dd6.Get<RTTI*>(0))));

			RTTI *f1 = &f;
			exd1.SetStorage(&f1, 1);
			dd6 = exd1;
			Assert::AreEqual(*dynamic_cast<Foo*>((exd1.Get<RTTI*>(0))), *dynamic_cast<Foo*>((dd6.Get<RTTI*>(0))));

			dd1 = d2;
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(d2.Get<float>(i), dd1.Get<float>(i));
			}
			dd1 = d3;
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(d3.Get<std::string>(i), dd1.Get<std::string>(i));
			}

			dd1 = d4;
			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(d4.Get<glm::vec4>(i) == dd1.Get<glm::vec4>(i));
			}

			dd1 = d5;
			dd5 = d5;
			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(d5.Get<glm::mat4x4>(i) == dd5.Get<glm::mat4x4>(i));
			}

			dd1 = d6;
			Assert::AreEqual(*dynamic_cast<Foo*>((d6.Get<RTTI*>(1))), *dynamic_cast<Foo*>((dd1.Get<RTTI*>(1))));
		}

		TEST_METHOD(OperatorEquality)
		{
			int a[] = { 3,6,7,32,6 };
			int aa[] = { 4,56,2,7,1 };
			Datum d1;
			Datum d1Alternative;
			for (int i = 0; i < 5; i++)
			{
				d1.PushBack(a[i]);
				d1Alternative.PushBack(aa[i]);
				Assert::AreEqual(a[i], d1.Get<int32_t>(i));
			}
			Assert::IsFalse(d1 == d1Alternative);
			Datum dd1(d1);
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(d1.Get<int32_t>(i), dd1.Get<int32_t>(i));
			}
			Assert::IsTrue(d1 == dd1);

			float b[] = { 5.6f,5.1f,6.5f,8.5f,7.7f };
			Datum d2;
			for (int i = 0; i < 5; i++)
			{
				d2.PushBack(b[i]);
				Assert::AreEqual(b[i], d2.Get<float>(i));
			}
			Datum dd2(d2);
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(d2.Get<float>(i), dd2.Get<float>(i));
			}
			Assert::IsTrue(d2 == dd2);

			std::string c[] = { "Hello", "World", "Greetings", "Cool", "C++" };
			std::string cc[] = { "C++", "C#", "Greetings", "Cool", "C" };
			Datum d3;
			Datum d3Alternative;
			for (int i = 0; i < 5; i++)
			{
				d3.PushBack(c[i]);
				d3Alternative.PushBack(cc[i]);
				Assert::AreEqual(c[i], d3.Get<std::string>(i));
			}
			Assert::IsFalse(d3 == d3Alternative);
			Datum dd3(d3);
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(d3.Get<std::string>(i), dd3.Get<std::string>(i));
			}
			Assert::IsTrue(d3 == dd3);

			glm::vec4 d[] = { { 4.5f, 6.3f, 2.4f, 1.0f },{ 0.5f,5.6f,8.54f, 5.6f },{ 5.6f,9.6f,1.2f,6.2f } };
			Datum d4;
			for (int i = 0; i < 3; i++)
			{
				d4.PushBack(d[i]);
				Assert::IsTrue(d[i] == d4.Get<glm::vec4>(i));
			}
			Datum dd4(d4);
			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(d4.Get<glm::vec4>(i) == dd4.Get<glm::vec4>(i));
			}
			Assert::IsTrue(d4 == dd4);

			glm::mat4x4 e[] = { { glm::mat4x4(1.0f) },{ glm::mat4x4(10.0f) },{ glm::mat4x4(5.0f) } };
			Datum d5;
			for (int i = 0; i < 3; i++)
			{
				d5.PushBack(e[i]);
				Assert::IsTrue(e[i] == d5.Get<glm::mat4x4>(i));
			}
			Datum dd5(d5);
			Assert::IsTrue(d5 == dd5);

			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(d5.Get<glm::mat4x4>(i) == dd5.Get<glm::mat4x4>(i));
			}

			Foo f(3, 5), g(4,7);
			Datum d6;
			d6.PushBack(&f);
			Assert::AreEqual(f, *dynamic_cast<Foo*>((d6.Get<RTTI*>(0))));

			Datum dd6(d6);
			Assert::AreEqual(*dynamic_cast<Foo*>((d6.Get<RTTI*>(0))), *dynamic_cast<Foo*>((dd6.Get<RTTI*>(0))));
			Assert::IsTrue(d6 == dd6);
			dd6.Set(&g, 0);
			Assert::IsFalse(d6 == dd6);
			dd6.PushBack(&f);
			Assert::IsFalse(d6 == dd6);
		}

		TEST_METHOD(OperatorInequality)
		{
			int a[] = { 3,6,7,32,6 };
			int aa[] = { 4,56,2,7,1 };
			Datum d1;
			Datum d1Alternative;
			for (int i = 0; i < 5; i++)
			{
				d1.PushBack(a[i]);
				d1Alternative.PushBack(aa[i]);
				Assert::AreEqual(a[i], d1.Get<int32_t>(i));
			}
			Assert::IsTrue(d1 != d1Alternative);
			Datum dd1(d1);
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(d1.Get<int32_t>(i), dd1.Get<int32_t>(i));
			}
			Assert::IsFalse(d1 != dd1);

			float b[] = { 5.6f,5.1f,6.5f,8.5f,7.7f };
			Datum d2;
			for (int i = 0; i < 5; i++)
			{
				d2.PushBack(b[i]);
				Assert::AreEqual(b[i], d2.Get<float>(i));
			}
			Datum dd2(d2);
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(d2.Get<float>(i), dd2.Get<float>(i));
			}
			Assert::IsFalse(d2 != dd2);

			std::string c[] = { "Hello", "World", "Greetings", "Cool", "C++" };
			std::string cc[] = { "C++", "C#", "Greetings", "Cool", "C" };
			Datum d3;
			Datum d3Alternative;
			for (int i = 0; i < 5; i++)
			{
				d3.PushBack(c[i]);
				d3Alternative.PushBack(cc[i]);
				Assert::AreEqual(c[i], d3.Get<std::string>(i));
			}
			Assert::IsTrue(d3 != d3Alternative);
			Datum dd3(d3);
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(d3.Get<std::string>(i), dd3.Get<std::string>(i));
			}
			Assert::IsFalse(d3 != dd3);

			glm::vec4 d[] = { { 4.5f, 6.3f, 2.4f, 1.0f },{ 0.5f,5.6f,8.54f, 5.6f },{ 5.6f,9.6f,1.2f,6.2f } };
			Datum d4;
			for (int i = 0; i < 3; i++)
			{
				d4.PushBack(d[i]);
				Assert::IsTrue(d[i] == d4.Get<glm::vec4>(i));
			}
			Datum dd4(d4);
			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(d4.Get<glm::vec4>(i) == dd4.Get<glm::vec4>(i));
			}
			Assert::IsFalse(d4 != dd4);

			glm::mat4x4 e[] = { { glm::mat4x4(1.0f) },{ glm::mat4x4(10.0f) },{ glm::mat4x4(5.0f) } };
			Datum d5;
			for (int i = 0; i < 3; i++)
			{
				d5.PushBack(e[i]);
				Assert::IsTrue(e[i] == d5.Get<glm::mat4x4>(i));
			}
			Datum dd5(d5);
			Assert::IsFalse(d5 != dd5);

			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(d5.Get<glm::mat4x4>(i) == dd5.Get<glm::mat4x4>(i));
			}

			Foo f(3, 5);
			Datum d6;
			d6.PushBack(&f);
			Assert::AreEqual(f, *dynamic_cast<Foo*>((d6.Get<RTTI*>(0))));

			Datum dd6(d6);
			Assert::AreEqual(*dynamic_cast<Foo*>((d6.Get<RTTI*>(0))), *dynamic_cast<Foo*>((dd6.Get<RTTI*>(0))));
			Assert::IsFalse(d6 != dd6);
		}

		TEST_METHOD(ScalarEquality)
		{
			Datum d1 = 5;
			Assert::IsTrue(d1 == 5);

			Datum d2 = std::string("Hello, Hollow World!");
			Assert::IsTrue(d2 == std::string("Hello, Hollow World!"));

			Datum d3 = 4.6f;
			Assert::IsTrue(d3 == 4.6f);

			Datum d4 = glm::vec4(2.5f,2.5f,2.5f,2.5f);
			Assert::IsTrue(d4 == glm::vec4(2.5f, 2.5f, 2.5f, 2.5f));

			Datum d5 = glm::mat4x4(3.6f);
			Assert::IsTrue(d5 == glm::mat4x4(3.6f));

			Foo a(5, 8);
			Datum d6 = &a;
			Assert::IsTrue(d6 == &a);
		}

		TEST_METHOD(ScalarNonScalarEqualityTest)
		{
			int a[] = { 3,6,7,32,6 };
			int aa[] = { 4,56,2,7,1 };
			Datum d1;
			Datum d1Alternative;
			for (int i = 0; i < 5; i++)
			{
				d1.PushBack(a[i]);
				d1Alternative.PushBack(aa[i]);
				Assert::AreEqual(a[i], d1.Get<int32_t>(i));
			}
			Assert::IsFalse(d1 == d1Alternative);
			Datum dd1(d1);
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(d1.Get<int32_t>(i), dd1.Get<int32_t>(i));
			}
			Assert::IsTrue(d1 == dd1);

			float b[] = { 5.6f,5.1f,6.5f,8.5f,7.7f };
			Datum d2;
			for (int i = 0; i < 5; i++)
			{
				d2.PushBack(b[i]);
				Assert::AreEqual(b[i], d2.Get<float>(i));
			}
			Datum dd2(d2);
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(d2.Get<float>(i), dd2.Get<float>(i));
			}
			Assert::IsTrue(d2 == dd2);

			std::string c[] = { "Hello", "World", "Greetings", "Cool", "C++" };
			std::string cc[] = { "C++", "C#", "Greetings", "Cool", "C" };
			Datum d3;
			Datum d3Alternative;
			for (int i = 0; i < 5; i++)
			{
				d3.PushBack(c[i]);
				d3Alternative.PushBack(cc[i]);
				Assert::AreEqual(c[i], d3.Get<std::string>(i));
			}
			Assert::IsFalse(d3 == d3Alternative);
			Datum dd3(d3);
			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(d3.Get<std::string>(i), dd3.Get<std::string>(i));
			}
			Assert::IsTrue(d3 == dd3);

			glm::vec4 d[] = { { 4.5f, 6.3f, 2.4f, 1.0f },{ 0.5f,5.6f,8.54f, 5.6f },{ 5.6f,9.6f,1.2f,6.2f } };
			Datum d4;
			for (int i = 0; i < 3; i++)
			{
				d4.PushBack(d[i]);
				Assert::IsTrue(d[i] == d4.Get<glm::vec4>(i));
			}
			Datum dd4(d4);
			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(d4.Get<glm::vec4>(i) == dd4.Get<glm::vec4>(i));
			}
			Assert::IsTrue(d4 == dd4);

			glm::mat4x4 e[] = { { glm::mat4x4(1.0f) },{ glm::mat4x4(10.0f) },{ glm::mat4x4(5.0f) } };
			Datum d5;
			for (int i = 0; i < 3; i++)
			{
				d5.PushBack(e[i]);
				Assert::IsTrue(e[i] == d5.Get<glm::mat4x4>(i));
			}
			Datum dd5(d5);
			Assert::IsTrue(d5 == dd5);

			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(d5.Get<glm::mat4x4>(i) == dd5.Get<glm::mat4x4>(i));
			}

			Foo f(3, 5), g(4, 7);
			Datum d6;
			d6.PushBack(&f);
			Assert::AreEqual(f, *dynamic_cast<Foo*>((d6.Get<RTTI*>(0))));

			Datum dd6(d6);
			Assert::AreEqual(*dynamic_cast<Foo*>((d6.Get<RTTI*>(0))), *dynamic_cast<Foo*>((dd6.Get<RTTI*>(0))));
			Assert::IsTrue(d6 == dd6);
			dd6.Set(&g, 0);
			Assert::IsFalse(d6 == dd6);
			dd6.PushBack(&f);
			Assert::IsFalse(d6 == dd6);

			Assert::IsTrue(d1 != 5);
			Assert::IsTrue(d2 != std::string("Hello, Hollow World!"));
			Assert::IsTrue(d3 != 4.6f);
			Assert::IsTrue(d4 != glm::vec4(2.5f, 2.5f, 2.5f, 2.5f));
			Assert::IsTrue(d5 != glm::mat4x4(3.6f));
			Assert::IsTrue(dd6 != &g);
		}

		TEST_METHOD(ScalarInequality)
		{
			Datum d1 = 5;
			Assert::IsFalse(d1 != 5);

			Datum d2 = std::string("Hello, Hollow World!");
			Assert::IsFalse(d2 != std::string("Hello, Hollow World!"));

			Datum d3 = 4.6f;
			Assert::IsTrue(d3 != 43.6f);

			Datum d4 = glm::vec4(2.5f, 2.5f, 2.5f, 2.5f);
			Assert::IsTrue(d4 != glm::vec4(2.0f, 2.5f, 2.5f, 2.5f));

			Datum d5 = glm::mat4x4(3.6f);
			Assert::IsTrue(d5 != glm::mat4x4(1.6f));

			Foo a(5, 8), b(3,5);
			Datum d6 = &a;
			Assert::IsTrue(d6 != &b);

		}

		TEST_METHOD(PopBack)
		{
			int a[] = { 3,6,7,32,6 };
			Datum d1;
			for (int i = 0; i < 5; i++)
			{
				d1.PushBack(a[i]);
				Assert::AreEqual(a[i], d1.Get<int32_t>(i));
			}
			d1.PopBack();
			Assert::AreEqual(a[3], d1.Get<int32_t>(d1.Length() - 1));
			d1.PopBack();
			Assert::AreEqual(a[2], d1.Get<int32_t>(d1.Length() - 1));

			float b[] = { 5.6f,5.1f,6.5f,8.5f,7.7f };
			Datum d2;
			for (int i = 0; i < 5; i++)
			{
				d2.PushBack(b[i]);
				Assert::AreEqual(b[i], d2.Get<float>(i));
			}
			d2.PopBack();
			Assert::AreEqual(b[3], d2.Get<float>(d2.Length() - 1));

			std::string c[] = { "Hello", "World", "Greetings", "Cool", "C++" };
			Datum d3;
			for (int i = 0; i < 5; i++)
			{
				d3.PushBack(c[i]);
				Assert::AreEqual(c[i], d3.Get<std::string>(i));
			}
			d3.PopBack();
			Assert::AreEqual(c[3], d3.Get<std::string>(d3.Length()-1));

			glm::vec4 d[] = { { 4.5f, 6.3f, 2.4f, 1.0f },{ 0.5f,5.6f,8.54f, 5.6f },{ 5.6f,9.6f,1.2f,6.2f } };
			Datum d4;
			for (int i = 0; i < 3; i++)
			{
				d4.PushBack(d[i]);
				Assert::IsTrue(d[i] == d4.Get<glm::vec4>(i));
			}
			d4.PopBack();
			Assert::IsTrue(d[1] == d4.Get<glm::vec4>(d4.Length() - 1));

			glm::mat4x4 e[] = { { glm::mat4x4(1.0f) },{ glm::mat4x4(10.0f) },{ glm::mat4x4(5.0f) } };
			Datum d5;
			for (int i = 0; i < 3; i++)
			{
				d5.PushBack(e[i]);
				Assert::IsTrue(e[i] == d5.Get<glm::mat4x4>(i));
			}
			d5.PopBack();
			Assert::IsTrue(e[1] == d5.Get<glm::mat4x4>(d5.Length() - 1));

			Foo f(3, 5);
			Datum d6;
			d6.PushBack(&f);
			Assert::AreEqual(f, *dynamic_cast<Foo*>((d6.Get<RTTI*>(0))));
			d6.PopBack();
			Assert::AreEqual(0U, d6.Length());
		}

		TEST_METHOD(Resize)
		{
			Datum d1;
			d1.SetType(Datum::DatumType::Integer);
			d1.Resize(5U);
			d1.Set(3, 0);
			d1.Set(4, 1);
			Assert::AreEqual(5U, d1.Length());
			Assert::AreEqual(4, d1.Get<int32_t>(1));
			d1.PushBack(5);
			d1.PushBack(5);
			d1.PushBack(5);
			d1.Resize(3U);
			Assert::AreEqual(3U, d1.Length());
			Assert::AreEqual(4, d1.Get<int32_t>(1));

			Datum d2;
			d2.SetType(Datum::DatumType::String);
			d2.Resize(5U);
			d2.Set(std::string("Hello World"));
			Assert::AreEqual(5U, d2.Length());
			Assert::AreEqual(std::string("Hello World"), d2.Get<std::string>());
			d2.PushBack(std::string("C"));
			d2.PushBack(std::string("C++"));
			d2.PushBack(std::string("C#"));
			d2.Resize(2U);
		}

		TEST_METHOD(ToString)
		{
			int a[] = { 3,6,7,32,6 };
			Datum d1;
			for (int i = 0; i < 5; i++)
			{
				d1.PushBack(a[i]);
				Assert::AreEqual(a[i], d1.Get<int32_t>(i));
			}
			Assert::AreEqual(std::to_string(a[2]), d1.ToString(2));

			float b[] = { 5.6f,5.1f,6.5f,8.5f,7.7f };
			Datum d2;
			for (int i = 0; i < 5; i++)
			{
				d2.PushBack(b[i]);
				Assert::AreEqual(b[i], d2.Get<float>(i));
			}
			Assert::AreEqual(std::to_string(b[2]), d2.ToString(2));

			std::string c[] = { "Hello", "World", "Greetings", "Cool", "C++" };
			Datum d3;
			for (int i = 0; i < 5; i++)
			{
				d3.PushBack(c[i]);
				Assert::AreEqual(c[i], d3.Get<std::string>(i));
			}
			Assert::AreEqual(c[2], d3.ToString(2));

			glm::vec4 d[] = { { 4.5f, 6.3f, 2.4f, 1.0f },{ 0.5f,5.6f,8.54f, 5.6f },{ 5.6f,9.6f,1.2f,6.2f } };
			Datum d4;
			for (int i = 0; i < 3; i++)
			{
				d4.PushBack(d[i]);
				Assert::IsTrue(d[i] == d4.Get<glm::vec4>(i));
			}
			Assert::AreEqual(glm::to_string(d[2]), d4.ToString(2));

			glm::mat4x4 e[] = { { glm::mat4x4(1.0f) },{ glm::mat4x4(10.0f) },{ glm::mat4x4(5.0f) } };
			Datum d5;
			for (int i = 0; i < 3; i++)
			{
				d5.PushBack(e[i]);
				Assert::IsTrue(e[i] == d5.Get<glm::mat4x4>(i));
			}
			Assert::AreEqual(glm::to_string(e[2]), d5.ToString(2));

			Foo f(3, 5);
			Datum d6;
			d6.PushBack(&f);
			Assert::AreEqual(f, *dynamic_cast<Foo*>((d6.Get<RTTI*>(0))));
		}

		TEST_METHOD(SetFromString)
		{
			int a[] = { 3,6,7,32,6 };
			Datum d1;
			for (int i = 0; i < 5; i++)
			{
				d1.PushBack(a[i]);
				Assert::AreEqual(a[i], d1.Get<int32_t>(i));
			}
			d1.SetFromString("47", 3);
			Assert::AreEqual(47, d1.Get<int32_t>(3));

			float b[] = { 5.6f,5.1f,6.5f,8.5f,7.7f };
			Datum d2;
			for (int i = 0; i < 5; i++)
			{
				d2.PushBack(b[i]);
				Assert::AreEqual(b[i], d2.Get<float>(i));
			}
			d2.SetFromString("2.3f", 1);
			Assert::AreEqual(2.3f, d2.Get<float>(1));

			std::string c[] = { "Hello", "World", "Greetings", "Cool", "C++" };
			Datum d3;
			for (int i = 0; i < 5; i++)
			{
				d3.PushBack(c[i]);
				Assert::AreEqual(c[i], d3.Get<std::string>(i));
			}
			d3.SetFromString("Hello Hello");
			Assert::AreEqual(std::string("Hello Hello"), d3.Get<std::string>(0));

			glm::vec4 d[] = { { 4.5f, 6.3f, 2.4f, 1.0f },{ 0.5f,5.6f,8.54f, 5.6f },{ 5.6f,9.6f,1.2f,6.2f } };
			Datum d4;
			for (int i = 0; i < 3; i++)
			{
				d4.PushBack(d[i]);
				Assert::IsTrue(d[i] == d4.Get<glm::vec4>(i));
			}
			d4.SetFromString("vec4(5.6355 , 4.7345 , 6.443 , 6.3242)");
			Assert::IsTrue(glm::vec4(5.6355f, 4.7345f, 6.443f, 6.3242f) == d4.Get<glm::vec4>(0));

			glm::mat4x4 e[] = { { glm::mat4x4(1.0f) },{ glm::mat4x4(10.0f) },{ glm::mat4x4(5.0f) } };
			Datum d5;
			for (int i = 0; i < 3; i++)
			{
				d5.PushBack(e[i]);
				Assert::IsTrue(e[i] == d5.Get<glm::mat4x4>(i));
			}
			d5.SetFromString(glm::to_string(e[2]));
			Assert::IsTrue(e[2] == d5.Get<glm::mat4x4>(0));

			Foo f(3, 5);
			Datum d6;
			d6.PushBack(&f);
			Assert::AreEqual(f, *((d6.Get<RTTI*>(0))->As<Foo>()));
		}

		TEST_METHOD(ExceptionTesting)
		{
			std::function<void()> Expression;

			Datum d1;
			const Datum& dd1 = d1;
			d1.SetType(Datum::DatumType::String);
			Expression = [&d1](){d1.SetType(Datum::DatumType::Float); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&d1]() {d1.Set(std::string("Hello"), 3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&d1]() {d1.Get<int32_t>(3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&d1]() {d1.Get<float>(3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&d1]() {d1.Get<std::string>(3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&d1]() {d1.Get<glm::vec4>(3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&d1]() {d1.Get<glm::mat4x4>(3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&d1]() {d1.Get<RTTI*>(3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&d1]() {d1.Set(std::string("Hello"), 3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&dd1]() {dd1.Get<int32_t>(3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&dd1]() {dd1.Get<float>(3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&dd1]() {dd1.Get<std::string>(3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&dd1]() {dd1.Get<glm::vec4>(3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&dd1]() {dd1.Get<glm::mat4x4>(3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&dd1]() {dd1.Get<RTTI*>(3); };
			Assert::ExpectException<std::exception>(Expression);
			
			d1.PushBack(std::string("Hello World"));

			Expression = [&d1]() {d1.ToString(3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&d1]() {d1.SetFromString("Hello" ,3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&d1]() {d1.Set(3, 3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&d1]() {d1.Set(4.6f, 3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&d1]() {d1.Set(glm::vec4(4.5f, 3.7f, 3.9f, 0.5f), 3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&d1]() {d1.Set(glm::mat4x4(5.6f), 3); };
			Assert::ExpectException<std::exception>(Expression);

			Foo f(3, 5);
			Datum d6;
			d6.PushBack(&f);

			Expression = [&d1, &f]() {d1.Set(&f, 3); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&d6]() {d6.SetFromString("Hello", 0); };
			Assert::ExpectException<std::exception>(Expression);

			int aa[5] = { 2,6,7,3,8 };
			glm::vec4 d[] = { { 4.5f, 6.3f, 2.4f, 1.0f },{ 0.5f,5.6f,8.54f, 5.6f },{ 5.6f,9.6f,1.2f,6.2f } };
			std::string c[] = { "Hello", "World", "Greetings", "Cool", "C++" };
			glm::mat4x4 e[] = { { glm::mat4x4(1.0f) },{ glm::mat4x4(10.0f) },{ glm::mat4x4(5.0f) } };
			float b[] = { 5.6f,5.1f,6.5f,8.5f,7.7f };
			RTTI *f1 = new Foo(3, 6);

			Datum exd1;

			Expression = [&exd1]() {exd1.Find(4); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1]() {exd1.Find(4.7f); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1]() {exd1.Find(std::string("C++")); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1]() {exd1.Find(glm::vec4()); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1]() {exd1.Find(glm::mat4x4()); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1, &f]() {exd1.Find(&f); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1, &aa]() {exd1.SetStorage(aa, 0); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1, &c]() {exd1.SetStorage(c, 0); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1, &d]() {exd1.SetStorage(d, 0); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1, &e]() {exd1.SetStorage(e, 0); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1, &b]() {exd1.SetStorage(b, 0); };
			Assert::ExpectException<std::exception>(Expression);


			Expression = [&exd1, &f1]() {exd1.SetStorage(&f1, 0); };
			Assert::ExpectException<std::exception>(Expression);

			exd1.SetStorage(aa, 5);

			Expression = [&exd1]() {exd1.Remove(4); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1]() {exd1.Remove(4.5f); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1]() {exd1.Remove(std::string("C++")); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1]() {exd1.Remove(glm::vec4()); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1]() {exd1.Remove(glm::mat4x4()); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1, &f]() {exd1.Remove(&f); };
			Assert::ExpectException<std::exception>(Expression);

			/*Expression = [&exd1]() {exd1.Resize(4); };
			Assert::ExpectException<std::exception>(Expression);*/

			Expression = [&exd1]() {exd1.PushBack(4); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1]() {exd1.PushBack(4.4f); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1]() {exd1.PushBack(std::string("Hello")); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1]() {exd1.PushBack(glm::vec4()); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1]() {exd1.PushBack(glm::mat4x4(3.4f)); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1 , &f]() {exd1.PushBack(&f); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&exd1, &f]() {exd1.PopBack(); };
			Assert::ExpectException<std::exception>(Expression);

			Datum xD, xD2;
			xD.SetType(Datum::DatumType::Integer);
			xD2.SetType(Datum::DatumType::Float);

			Expression = [&xD]() {xD = 4.5f; };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&xD]() {xD = std::string("Hello"); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&xD]() {xD = glm::vec4(); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&xD]() {xD = glm::mat4x4(7.3f); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&xD2]() {xD2 = 7; };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&xD, &f]() {xD = &f; };
			Assert::ExpectException<std::exception>(Expression);

			delete f1;
		}

		TEST_METHOD(MoveSemantics)
		{
			Datum d1 = 6;
			d1.PushBack(8);
			Datum d2(std::move(d1));
			Assert::AreEqual(8, d2.Get<int32_t>(1));

			Datum d3;
			d3 = std::move(d2);
			Assert::AreEqual(8, d3.Get<int32_t>(1));
		}

		TEST_METHOD(Remove)
		{
			int a[] = { 3,6,7,32,6 };
			Datum d1;
			for (int i = 0; i < 5; i++)
			{
				d1.PushBack(a[i]);
				Assert::AreEqual(a[i], d1.Get<int32_t>(i));
			}
			d1.Remove(a[3]);
			Assert::AreEqual(a[4], d1.Get<int32_t>(3));
			Assert::AreEqual(4U, d1.Length());

			float b[] = { 5.6f,5.1f,6.5f,8.5f,7.7f };
			Datum d2;
			for (int i = 0; i < 5; i++)
			{
				d2.PushBack(b[i]);
				Assert::AreEqual(b[i], d2.Get<float>(i));
			}
			d2.Remove(b[3]);
			Assert::AreEqual(b[4], d2.Get<float>(3));
			Assert::AreEqual(4U, d2.Length());

			std::string c[] = { "Hello", "World", "Greetings", "Cool", "C++" };
			Datum d3;
			for (int i = 0; i < 5; i++)
			{
				d3.PushBack(c[i]);
				Assert::AreEqual(c[i], d3.Get<std::string>(i));
			}
			d3.Remove(c[3]);
			Assert::AreEqual(c[4], d3.Get<std::string>(3));
			Assert::AreEqual(4U, d3.Length());
			d3.RemoveWithIndex(0);
			Assert::AreEqual(c[1], d3.Get<std::string>(0));
			Assert::AreEqual(3U, d3.Length());

			glm::vec4 d[] = { { 4.5f, 6.3f, 2.4f, 1.0f },{ 0.5f,5.6f,8.54f, 5.6f },{ 5.6f,9.6f,1.2f,6.2f } };
			Datum d4;
			for (int i = 0; i < 3; i++)
			{
				d4.PushBack(d[i]);
				Assert::IsTrue(d[i] == d4.Get<glm::vec4>(i));
			}
			d4.Remove(d[0]);
			Assert::IsTrue(d[1] == d4.Get<glm::vec4>(0));
			Assert::AreEqual(2U, d4.Length());

			glm::mat4x4 e[] = { { glm::mat4x4(1.0f) },{ glm::mat4x4(10.0f) },{ glm::mat4x4(5.0f) } };
			Datum d5;
			for (int i = 0; i < 3; i++)
			{
				d5.PushBack(e[i]);
				Assert::IsTrue(e[i] == d5.Get<glm::mat4x4>(i));
			}
			d5.Remove(e[0]);
			Assert::IsTrue(e[1] == d5.Get<glm::mat4x4>(0));
			Assert::AreEqual(2U, d5.Length());

			Foo f(3, 5);
			Datum d6;
			d6.PushBack(&f);
			Assert::AreEqual(f, *dynamic_cast<Foo*>((d6.Get<RTTI*>(0))));
			d6.Remove(&f);
			Assert::AreEqual(0U, d6.Length());
		}
	};


	_CrtMemState DatumTest::sStartMemState;
}