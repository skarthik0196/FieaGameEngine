#pragma once
#include "pch.h"
#include "CppUnitTest.h"
#define _CRTDBG_MAP_ALLOC
#include<string>
#include<stdlib.h>
#include<crtdbg.h>
#include<exception>
#include<functional>
#include "Vector.h"
#include "Foo.h"
#include"SList.h"
#include"HashMap.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<UnitTestLibraryDesktop::Foo>(const UnitTestLibraryDesktop::Foo& T)
	{
		RETURN_WIDE_STRING(T.GetData());
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(HashMapTest)
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

		TEST_METHOD(HashMapInitialization)
		{
			HashMap<int, int> Map1;
			HashMap<int, int>::Iterator deadIterator = Map1.begin();

			HashMap<int, int> Map2 = { {4,5},{5,7},{8,3},{9,1},{1,7} };
			int data;
			Assert::AreEqual(true, Map2.ContainsKey(1,data));
			Assert::AreEqual(7, data);
			Assert::AreEqual(true, Map2.ContainsKey(4,data));
			Assert::AreEqual(5, data);
			Assert::AreEqual(true, Map2.ContainsKey(5,data));
			Assert::AreEqual(7, data);
			Assert::AreEqual(true, Map2.ContainsKey(8,data));
			Assert::AreEqual(3, data);

			HashMap<Foo, int> Map3;

			HashMap<std::string, std::string> Map4 = { {"Hello", "World"}, {"Qwerty" , "Uiop"}, {"Good","Morning"}, {"C++" ,"C++"} };

			std::string TempString = "Hello", TempString3="World";
			std::string TempString2;
			Assert::AreEqual(true, Map4.ContainsKey(TempString, TempString2));
			Assert::AreEqual(TempString3, TempString2);

			char *a = new char[30], *b = new char[30];
			strcpy_s(a, 30, TempString.c_str());
			strcpy_s(b, 30, TempString3.c_str());
			Foo f1(4, 6), f2(5, 7);
			HashMap<char*, Foo> Map5 = { {a,f1}, {b,f2} };
			Foo f3;
			Assert::AreEqual(true, Map5.ContainsKey(a, f3));
			Assert::AreEqual(f1, f3);
			Assert::AreEqual(true, Map5.ContainsKey(b, f3));
			Assert::AreEqual(f2, f3);

			delete a;
			delete b;
		}

		TEST_METHOD(HashMapInsert)
		{
			HashMap<int, int> Map1;
			HashMap<int, int>::Iterator It;
			std::pair<int, int> a = { 5,76 };
			Map1.Insert(a);
			a.second = 4;
			It= Map1.Insert(a);
			a.second = 76;
			Assert::AreEqual(a.second, (*It).second);
			It = Map1.Find(a.first);
			Assert::AreEqual(a.second, (*It).second);

			HashMap<Foo, int> Map3;
			std::pair<Foo, int> b = { Foo(4, 6),4 };
			HashMap<Foo, int>::Iterator It2;
			It2 = Map3.Insert(b);
			Assert::AreEqual(b.second, (*It2).second);
			It2 = Map3.Find(b.first);
			Assert::AreEqual(b.second, (*It2).second);

			HashMap<std::string, std::string> Map4 = { { "Hello", "World" },{ "Qwerty" , "Uiop" },{ "Good","Morning" },{ "C++" ,"C++" } };
			std::string TempString = "Greetings", TempString2 = "Mortal";
			HashMap<std::string, std::string>::Iterator It3;
			It3 = Map4.Insert(std::make_pair(TempString, TempString2));
			Assert::AreEqual(TempString2, (*It3).second);
			It3 = Map4.Find(TempString);
			Assert::AreEqual(TempString2, (*It3).second);

			bool Result;
			char *c = new char[30], *d = new char[30];
			strcpy_s(c, 30, TempString.c_str());
			strcpy_s(d, 30, TempString2.c_str());
			Foo f1(4, 6), f2(5, 7);
			HashMap<char*, Foo> Map5;
			HashMap<char*, Foo>::Iterator It5;
			Foo f3(576,34);
			Map5.Insert(std::make_pair(c, f1), Result);
			Map5.Insert(std::make_pair(d, f2), Result);
			It5 = Map5.Find(c);
			Assert::AreEqual(f1, (*It5).second);
			It5 = Map5.Find(d);
			Assert::AreEqual(f2, (*It5).second);

			delete c;
			delete d;
		}

		TEST_METHOD(UserDefinedHashAndComparatorFunctor)
		{
			HashMap<Foo, Foo, Foo, Foo> Map3;
			std::pair<Foo, Foo> a = { Foo(7,35), Foo(3,86) };
			std::pair<Foo, Foo> b = { Foo(4, 6), Foo(6,4) };
			std::pair<Foo, Foo> c = { Foo(2, 1), Foo(5,4) };
			HashMap<Foo, Foo, Foo, Foo>::Iterator It2;
			bool IsInserted = false;

			It2 = Map3.Insert(a);
			Assert::AreEqual(a.second, (*It2).second);

			It2 = Map3.Insert(b, IsInserted);
			Assert::AreEqual(b.second, (*It2).second);
			Assert::AreEqual(true, IsInserted);
			It2 = Map3.Find(b.first);
			Assert::AreEqual(b.second, (*It2).second);
			Assert::AreEqual(a.second, Map3[a.first]);

			It2 = Map3.Insert(c);
			Assert::AreEqual(c.second, (*It2).second);
		}

		TEST_METHOD(HashMapFind)
		{
			HashMap<int, int> Map1;
			HashMap<int, int>::Iterator It;
			std::pair<int, int> x = { 5,76 };
			Map1.Insert(x);
			Assert::AreEqual(x.second, (*Map1.Find(x.first)).second);

			HashMap<Foo, Foo, Foo, Foo> Map3;
			std::pair<Foo, Foo> a = { Foo(7,35), Foo(3,86) };
			std::pair<Foo, Foo> b = { Foo(4, 6), Foo(6,4) };
			std::pair<Foo, Foo> c = { Foo(2, 1), Foo(5,4) };
			HashMap<Foo, Foo, Foo, Foo>::Iterator It2;
			bool IsInserted = false;

			It2 = Map3.Insert(a);
			It2 = Map3.Insert(b, IsInserted);
			It2 = Map3.Insert(c);

			Assert::AreEqual(a.second, (*Map3.Find(a.first)).second);
			Assert::AreEqual(b.second, (*Map3.Find(b.first)).second);
			Assert::AreEqual(c.second, (*Map3.Find(c.first)).second);

			HashMap<std::string, std::string> Map4 = { { "Hello", "World" },{ "Qwerty" , "Uiop" },{ "Good","Morning" },{ "C++" ,"C++" } };
			std::string TempString = "Greetings", TempString2 = "Mortal";
			HashMap<std::string, std::string>::Iterator It3;

			It3 = Map4.Insert(std::make_pair(TempString, TempString2));
			
			TempString = "Uiop";
			TempString2 = "Qwerty";
			Assert::AreEqual(TempString, (*Map4.Find(TempString2)).second);
			
			TempString = "C++";
			TempString2 = "C++";
			Assert::AreEqual(TempString, (*Map4.Find(TempString2)).second);

			TempString = "Morning";
			TempString2 = "Good";
			Assert::AreEqual(TempString, (*Map4.Find(TempString2)).second);

			bool Result;
			char *y = new char[30], *z = new char[30];
			strcpy_s(y, 30, TempString.c_str());
			strcpy_s(z, 30, TempString2.c_str());
			Foo f1(4, 6), f2(5, 7);
			HashMap<char*, Foo> Map5;
			HashMap<char*, Foo>::Iterator It5;

			Map5.Insert(std::make_pair(y, f1), Result);
			Map5.Insert(std::make_pair(z, f2), Result);

			Assert::AreEqual(f1, (*Map5.Find(y)).second);
			Assert::AreEqual(f2, (*Map5.Find(z)).second);

			delete y;
			delete z;
		}

		TEST_METHOD(OperatorDereference)
		{
			HashMap<int, int> Map1;
			HashMap<int, int>::Iterator It;
			HashMap<int, int>::Iterator& It2 = It;
			std::pair<int, int> a = { 5,76 };
			std::pair<int, int> b = { 2,5 };
			It = Map1.Insert(a);

			Assert::AreEqual(a.first, (*It).first);
			Assert::AreEqual(a.first, It->first);
			Assert::AreEqual(a.second, It2->second);
			Map1[b.first] = b.second;
			Assert::AreEqual(b.second, Map1[b.first]);

			HashMap<Foo, Foo, Foo, Foo> Map3;
			std::pair<Foo, Foo> c = { Foo(7,35), Foo(3,86) };
			std::pair<Foo, Foo> d = { Foo(4, 6), Foo(6,4) };
			std::pair<Foo, Foo> e = { Foo(2, 1), Foo(5,4) };
			HashMap<Foo, Foo, Foo, Foo>::Iterator It3;
			bool IsInserted = false;

			It3 = Map3.Insert(c);
			It3 = Map3.Insert(d, IsInserted);
			It3 = Map3.Insert(e);

			Assert::AreEqual(c.second, Map3[c.first]);
			Assert::AreEqual(d.second, Map3[d.first]);
			Assert::AreEqual(e.second, Map3[e.first]);

			HashMap<std::string, std::string> Map4 = { { "Hello", "World" },{ "Qwerty" , "Uiop" },{ "Good","Morning" },{ "C++" ,"C++" } };
			std::string TempString = "C++", TempString2 = "C++";
			HashMap<std::string, std::string>::Iterator It4;

			Assert::AreEqual(TempString2, Map4[TempString]);
			TempString = "Good";
			TempString2 = "Morning";
			Assert::AreEqual(TempString2, Map4[TempString]);
			TempString = "Hello";
			TempString2 = "World";
			Assert::AreEqual(TempString2, Map4[TempString]);

			char *y = new char[30], *z = new char[30];
			strcpy_s(y, 30, TempString.c_str());
			strcpy_s(z, 30, TempString2.c_str());
			Foo f1(4, 6), f2(5, 7);
			HashMap<char*, Foo> Map5;
			HashMap<char*, Foo>::Iterator It5;

			Map5.Insert(std::make_pair(y, f1));
			Map5.Insert(std::make_pair(z, f2));

			Assert::AreEqual(f1, Map5[y]);
			Assert::AreEqual(f2, Map5[z]);
			
			delete y;
			delete z;
		}

		TEST_METHOD(ContainsKey)
		{
			HashMap<Foo, Foo, Foo, Foo> Map1;
			std::pair<Foo, Foo> c = { Foo(7,35), Foo(3,86) };
			std::pair<Foo, Foo> d = { Foo(4, 6), Foo(6,4) };
			std::pair<Foo, Foo> e = { Foo(2, 1), Foo(5,4) };
			HashMap<Foo, Foo, Foo, Foo>::Iterator It3;
			bool IsInserted = false;

			It3 = Map1.Insert(c);
			It3 = Map1.Insert(d, IsInserted);
			It3 = Map1.Insert(e);

			Assert::AreEqual(true, Map1.ContainsKey(c.first));
			Assert::AreEqual(true, Map1.ContainsKey(d.first));
			Assert::AreEqual(true, Map1.ContainsKey(e.first));
			Assert::AreEqual(false, Map1.ContainsKey(Foo(432, 542)));

			HashMap<int, int> Map2 = { { 4,5 },{ 5,7 },{ 8,3 },{ 9,1 },{ 1,7 } };
			int data;
			Assert::AreEqual(true, Map2.ContainsKey(1, data));
			Assert::AreEqual(7, data);
			Assert::AreEqual(true, Map2.ContainsKey(4, data));
			Assert::AreEqual(5, data);
			Assert::AreEqual(true, Map2.ContainsKey(5, data));
			Assert::AreEqual(7, data);
			Assert::AreEqual(true, Map2.ContainsKey(8, data));
			Assert::AreEqual(3, data);
			Assert::AreEqual(false, Map2.ContainsKey(75, data));

			HashMap<Foo, int> Map3;

			HashMap<std::string, std::string> Map4 = { { "Hello", "World" },{ "Qwerty" , "Uiop" },{ "Good","Morning" },{ "C++" ,"C++" } };

			std::string TempString = "Hello", TempString3 = "World";
			std::string TempString2;
			Assert::AreEqual(true, Map4.ContainsKey(TempString, TempString2));
			Assert::AreEqual(TempString3, TempString2);

			char *a = new char[30], *b = new char[30];

			strcpy_s(a, 30, TempString.c_str());
			strcpy_s(b, 30, TempString3.c_str());
			Foo f1(4, 6), f2(5, 7);
			HashMap<char*, Foo> Map5 = { { a,f1 },{ b,f2 } };
			Foo f3;
			Assert::AreEqual(true, Map5.ContainsKey(a, f3));
			Assert::AreEqual(f1, f3);
			Assert::AreEqual(true, Map5.ContainsKey(b, f3));
			Assert::AreEqual(f2, f3);

			delete a;
			delete b;
		}

		TEST_METHOD(Clear)
		{
			std::pair<int, int> a = { 5,76 };
			std::pair<int, int> b = { 2,5 };
			HashMap<int, int> Map1;
			HashMap<int, int>::Iterator It;

			It = Map1.Insert(a);
			It = Map1.Insert(b);

			Assert::AreEqual(true, Map1.ContainsKey(a.first));
			Assert::AreEqual(true, Map1.ContainsKey(b.first));

			Map1.Clear();

			Assert::AreEqual(false, Map1.ContainsKey(a.first));
			Assert::AreEqual(false, Map1.ContainsKey(b.first));

			HashMap<Foo, Foo, Foo, Foo> Map3;
			std::pair<Foo, Foo> c = { Foo(7,35), Foo(3,86) };
			std::pair<Foo, Foo> d = { Foo(4, 6), Foo(6,4) };
			std::pair<Foo, Foo> e = { Foo(2, 1), Foo(5,4) };
			HashMap<Foo, Foo, Foo, Foo>::Iterator It3;
			bool IsInserted = false;

			It3 = Map3.Insert(c);
			It3 = Map3.Insert(d, IsInserted);
			It3 = Map3.Insert(e);

			Map3.Clear();
			Assert::AreEqual(false, Map3.ContainsKey(c.first));
			Assert::AreEqual(false, Map3.ContainsKey(d.first));
			Assert::AreEqual(false, Map3.ContainsKey(e.first));

			HashMap<std::string, std::string> Map4 = { { "Hello", "World" },{ "Qwerty" , "Uiop" },{ "Good","Morning" },{ "C++" ,"C++" } };

			std::string TempString = "Hello", TempString3 = "Qwerty";
			std::string TempString2;
			Assert::AreEqual(true, Map4.ContainsKey(TempString, TempString2));

			Map4.Clear();
			Assert::AreEqual(false, Map4.ContainsKey(TempString));
			Assert::AreEqual(false, Map4.ContainsKey(TempString3));


			char *x = new char[30], *y = new char[30];

			strcpy_s(x, 30, TempString.c_str());
			strcpy_s(y, 30, TempString3.c_str());
			Foo f1(4, 6), f2(5, 7);
			HashMap<char*, Foo> Map5 = { { x,f1 },{ y,f2 } };
			Foo f3;
			Assert::AreEqual(true, Map5.ContainsKey(x, f3));
			Assert::AreEqual(f1, f3);
			Assert::AreEqual(true, Map5.ContainsKey(y, f3));
			Assert::AreEqual(f2, f3);

			Map5.Clear();

			Assert::AreEqual(false, Map5.ContainsKey(x));
			Assert::AreEqual(false, Map5.ContainsKey(y));

			delete x;
			delete y;


		}

		TEST_METHOD(Remove)
		{
			std::pair<int, int> a = { 5,76 };
			std::pair<int, int> b = { 2,5 };
			HashMap<int, int> Map1;
			HashMap<int, int>::Iterator It;

			It = Map1.Insert(a);
			It = Map1.Insert(b);

			Map1.Remove(a.first);
			Assert::AreEqual(false, Map1.ContainsKey(a.first));

			HashMap<Foo, Foo, Foo, Foo> Map3;
			std::pair<Foo, Foo> c = { Foo(7,35), Foo(3,86) };
			std::pair<Foo, Foo> d = { Foo(4, 6), Foo(6,4) };
			std::pair<Foo, Foo> e = { Foo(2, 1), Foo(5,4) };
			HashMap<Foo, Foo, Foo, Foo>::Iterator It3;
			bool IsInserted = false;

			It3 = Map3.Insert(c);
			It3 = Map3.Insert(d, IsInserted);
			It3 = Map3.Insert(e);

			Map3.Remove(c.first);
			Assert::AreEqual(false, Map3.ContainsKey(c.first));
			Map3.Remove(d.first);
			Assert::AreEqual(false, Map3.ContainsKey(d.first));
			Map3.Remove(e.first);
			Assert::AreEqual(false, Map3.ContainsKey(e.first));

			HashMap<std::string, std::string> Map4 = { { "Hello", "World" },{ "Qwerty" , "Uiop" },{ "Good","Morning" },{ "C++" ,"C++" } };

			std::string TempString = "Hello", TempString3 = "Qwerty";
			std::string TempString2;
			Assert::AreEqual(true, Map4.ContainsKey(TempString, TempString2));
			Map4.Remove(TempString);
			Assert::AreEqual(false, Map4.ContainsKey(TempString));
			Map4.Remove(TempString3);
			Assert::AreEqual(false, Map4.ContainsKey(TempString3));

			char *x = new char[30], *y = new char[30];
			strcpy_s(x, 30, TempString.c_str());
			strcpy_s(y, 30, TempString3.c_str());
			Foo f1(4, 6), f2(5, 7);
			HashMap<char*, Foo> Map5 = { { x,f1 },{ y,f2 } };
			Foo f3;
			Assert::AreEqual(true, Map5.ContainsKey(x, f3));
			Assert::AreEqual(f1, f3);
			Assert::AreEqual(true, Map5.ContainsKey(y, f3));

			Map5.Remove(x);
			Assert::AreEqual(false, Map5.ContainsKey(x));
			Map5.Remove(y);
			Assert::AreEqual(false, Map5.ContainsKey(y));

			HashMap<int, int> Map6 = { { 3,4 } };
			Map6.Insert(std::make_pair(8, 6));
			Map6.Insert(std::make_pair(5, 7));
			Map6.Remove(8);
			Assert::AreEqual(false, Map6.ContainsKey(8));
			Map6.Remove(3);
			Assert::AreEqual(false, Map6.ContainsKey(3));

			delete x;
			delete y;
		}

		TEST_METHOD(OperatorIncrement)
		{
			std::pair<int, int> a = { 5,76 };
			std::pair<int, int> b = { 2,5 };
			std::pair<int, int> c = { 3,7 };
			std::pair<int, int> d = { 1,8 };
			std::pair<int, int> e = { 23,56 };
			std::pair<int, int> f = { 14,34 };
			std::pair<int, int> g = { 52,2 };
			uint32_t count = 0;;
			HashMap<int, int> Map1(3);
			HashMap<int, int>::Iterator It;

			It = Map1.Insert(a);
			It = Map1.Insert(b);
			It = Map1.Insert(c);
			It = Map1.Insert(d);
			It = Map1.Insert(e);
			It = Map1.Insert(f);
			It = Map1.Insert(g);

			It = Map1.begin();
			for (; It != Map1.end(); ++It)
			{
				Assert::AreEqual(true, Map1.ContainsKey((*It).first));
				count++;
			}
			Assert::AreEqual(Map1.Length(), count);
			It = Map1.begin();
			count = 0;
			for (; It != Map1.end();It++)
			{
				Assert::AreEqual(true, Map1.ContainsKey((*It).first));
				count++;
			}
			Assert::AreEqual(Map1.Length(), count);

			HashMap<std::string, std::string> Map4 = { { "Hello", "World" },{ "Qwerty" , "Uiop" },{ "Good","Morning" },{ "C++" ,"C++" } };
			HashMap<std::string, std::string>::Iterator It4 = Map4.begin();
			HashMap<Foo, Foo, Foo, Foo> Map3;
			std::pair<Foo, Foo> h = { Foo(7,35), Foo(3,86) };
			std::pair<Foo, Foo> i = { Foo(4, 6), Foo(6,4) };
			std::pair<Foo, Foo> j = { Foo(2, 1), Foo(5,4) };
			HashMap<Foo, Foo, Foo, Foo>::Iterator It3;
			bool IsInserted = false;

			It3 = Map3.Insert(h);
			It3 = Map3.Insert(i, IsInserted);
			It3 = Map3.Insert(j);

			count = 0;
			It3 = Map3.begin();
			for (; It3 != Map3.end(); ++It3)
			{
				Assert::AreEqual(true, Map3.ContainsKey((*It3).first));
				count++;
			}
			Assert::AreEqual(count, Map3.Length());

			count = 0;
			for (; It4 != Map4.end(); ++It4)
			{
				Assert::AreEqual(true, Map4.ContainsKey((*It4).first));
				++count;
			}
			Assert::AreEqual(count, Map4.Length());

			char *x = new char[30], *y = new char[30];
			std::string TempString = "Hello", TempString3 = "Qwerty";
			strcpy_s(x, 30, TempString.c_str());
			strcpy_s(y, 30, TempString3.c_str());
			Foo f1(4, 6), f2(5, 7);
			HashMap<char*, Foo> Map5 = { { x,f1 },{ y,f2 } };
			HashMap<char*, Foo>::Iterator It5= Map5.begin();
			
			count = 0;
			for (; It5 != Map5.end(); ++It5)
			{
				Assert::AreEqual(true, Map5.ContainsKey((*It5).first));
				++count;
			}
			Assert::AreEqual(Map5.Length(), count);

			delete x;
			delete y;
		}

		TEST_METHOD(At)
		{
			std::pair<int, int> a = { 5,76 };
			std::pair<int, int> b = { 2,5 };
			HashMap<int, int> Map1;
			const HashMap<int, int>& Map2 = Map1;
			HashMap<int, int>::Iterator It;

			It = Map1.Insert(a);
			It = Map1.Insert(b);

			Assert::AreEqual(a.second, Map1.At(a.first));
			Assert::AreEqual(b.second, Map1.At(b.first));
			Assert::AreEqual(a.second, Map2.At(a.first));
			Assert::AreEqual(b.second, Map2.At(b.first));

			HashMap<Foo, Foo, Foo, Foo> Map3;
			std::pair<Foo, Foo> c = { Foo(7,35), Foo(3,86) };
			std::pair<Foo, Foo> d = { Foo(4, 6), Foo(6,4) };
			std::pair<Foo, Foo> e = { Foo(2, 1), Foo(5,4) };
			HashMap<Foo, Foo, Foo, Foo>::Iterator It3;
			bool IsInserted = false;

			It3 = Map3.Insert(c);
			It3 = Map3.Insert(d, IsInserted);
			It3 = Map3.Insert(e);

			Assert::AreEqual(c.second, Map3.At(c.first));
			Assert::AreEqual(d.second, Map3.At(d.first));
			Assert::AreEqual(e.second, Map3.At(e.first));

			Foo f(6, 3);
			Map3.At(c.first) = f;

			Assert::AreEqual(f, Map3.At(c.first));

			HashMap<std::string, std::string> Map4 = { { "Hello", "World" },{ "Qwerty" , "Uiop" },{ "Good","Morning" },{ "C++" ,"C++" } };

			std::string TempString = "Hello", TempString2 = "World", TempString3= "Qwerty", TempString4 = "Uiop";
			Assert::AreEqual(TempString2, Map4.At(TempString));
			Assert::AreEqual(TempString4, Map4.At(TempString3));

			char *x = new char[30], *y = new char[30];
			strcpy_s(x, 30, TempString.c_str());
			strcpy_s(y, 30, TempString3.c_str());
			Foo f1(4, 6), f2(5, 7);
			HashMap<char*, Foo> Map5 = { { x,f1 },{ y,f2 } };

			Assert::AreEqual(f1, Map5.At(x));
			Assert::AreEqual(f2, Map5.At(y));

			delete x;
			delete y;
		}

		TEST_METHOD(ExceptionTesting)
		{
			std::function<void()> Expression = []() {HashMap<std::string, int>Map1(0); };
			Assert::ExpectException<std::exception>(Expression);
			HashMap<int, int>::Iterator It;
			Expression = [&It]() {*It; };
			Assert::ExpectException<std::exception>(Expression);
			Expression = [&It]() {It.operator->(); };
			Assert::ExpectException<std::exception>(Expression);
			Expression = [&It]() {++It; };
			Assert::ExpectException<std::exception>(Expression);
			Expression = [&It]() {It++; };
			Assert::ExpectException<std::exception>(Expression);

			HashMap<int, int> Map;
			Expression = [&Map]() {Map.At(4);}; 
			Assert::ExpectException<std::exception>(Expression);
			const HashMap<int, int>& ConstMap = Map;
			Expression = [&ConstMap]() {ConstMap.At(4); };
			Assert::ExpectException<std::exception>(Expression);
		}
	};

	_CrtMemState HashMapTest::sStartMemState;
}