#pragma once
#include "pch.h"
#include "CppUnitTest.h"
#define _CRTDBG_MAP_ALLOC
#include<string>
#include<stdlib.h>
#include<crtdbg.h>
#include<exception>
#include<functional>
#include "SList.h"
#include "Foo.h"

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
	TEST_CLASS(SListTest)
	{
	private:
		SList<int> IntList;
		SList<int *> IntPointerList;
		SList<float> FloatList;
		SList<std::string> StringList;
		SList<Foo> FooList;
		SList<Foo*> FooPointerList;

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
		/// <summary>
		/// Tests creation of lists with primitive data types
		/// </summary>
		TEST_METHOD(ListCreationPrimitive)
		{
			
			Assert::AreEqual(true, IntList.IsEmpty());
			Assert::AreEqual(0, IntList.Length());
			
			Assert::AreEqual(true, IntPointerList.IsEmpty());
			Assert::AreEqual(0, IntPointerList.Length());

			Assert::AreEqual(true, FloatList.IsEmpty());
			Assert::AreEqual(0, FloatList .Length());

			Assert::AreEqual(true, StringList.IsEmpty());
			Assert::AreEqual(0, StringList.Length());
		}

		/// <summary>
		/// Tests creation of lists with user defined data types
		/// </summary>
		TEST_METHOD(ListCreationUserDefined)
		{
			
			Assert::AreEqual(true, FooList.IsEmpty());
			Assert::AreEqual(0, FooList.Length());

			Assert::AreEqual(true, FooPointerList.IsEmpty());
			Assert::AreEqual(0, FooPointerList.Length());
		}

		/// <summary>
		/// Tests Pushback method of lists with 6 different data tyoes
		/// </summary>
		TEST_METHOD(PushBack)
		{
			int x = 5;
			int y = 7;
			//Test Pushback on Int List
			SList<int>::Iterator It;
			It=IntList.PushBack(x);
			Assert::AreEqual(x, *It);
			Assert::AreEqual(x, IntList.Front());
			Assert::AreEqual(x, IntList.Back());
			Assert::AreEqual(1, IntList.Length());
			
			IntList.PushBack(y);
			Assert::AreEqual(x, IntList.Front());
			Assert::AreEqual(y, IntList.Back());
			Assert::AreEqual(2, IntList.Length());

			IntList.Clear();
			
			//Test Pushback on Int* list
			int *Temp = &x;

			IntPointerList.PushBack(Temp);
			Assert::AreEqual(Temp, IntPointerList.Front());
			Assert::AreEqual(Temp, IntPointerList.Back());
			Assert::AreEqual(1, IntPointerList.Length());

			int *Temp2 = &y;

			IntPointerList.PushBack(Temp2);
			Assert::AreEqual(Temp, IntPointerList.Front());
			Assert::AreEqual(Temp2, IntPointerList.Back());
			Assert::AreEqual(2, IntPointerList.Length());
			IntPointerList.Clear();

			//Test Pushback on Float List
			float f1 = 7.0f;
			float f2 = 8.0f;
			FloatList.PushBack(f1);
			Assert::AreEqual(f1, FloatList.Front());
			Assert::AreEqual(f1, FloatList.Back());
			Assert::AreEqual(1, FloatList.Length());

			FloatList.PushBack(f2);
			Assert::AreEqual(f1, FloatList.Front());
			Assert::AreEqual(f2, FloatList.Back());
			Assert::AreEqual(2, FloatList.Length());

			FloatList.Clear();

			//Test Pushback on String list
			std::string Word = "Hello";
			StringList.PushBack(Word);
			Assert::AreEqual(Word, StringList.Front());
			Assert::AreEqual(Word, StringList.Back());
			Assert::AreEqual(1, StringList.Length());

			std::string Word2 = "World";
			StringList.PushBack(Word2);
			Assert::AreEqual(Word, StringList.Front());
			Assert::AreEqual(Word2, StringList.Back());
			Assert::AreEqual(2, StringList.Length());

			StringList.Clear();

			//Test Pushback on UserDefined Foo class
			Foo a;
			FooList.PushBack(a);
			Assert::AreEqual(a, FooList.Front());
			Assert::AreEqual(a, FooList.Back());
			Assert::AreEqual(1, FooList.Length());

			Foo b(5);
			FooList.PushBack(b);
			Assert::AreEqual(a, FooList.Front());
			Assert::AreEqual(b, FooList.Back());
			Assert::AreEqual(2, FooList.Length());

			FooList.Clear();

			//Test Pushback on a pointer to User Defined Foo Class
			Foo *c= &a;
			FooPointerList.PushBack(c);
			Assert::AreEqual(c, FooPointerList.Front());
			Assert::AreEqual(c, FooPointerList.Back());
			Assert::AreEqual(1, FooPointerList.Length());

			Foo *d = &b;
			FooPointerList.PushBack(d);
			Assert::AreEqual(c, FooPointerList.Front());
			Assert::AreEqual(d, FooPointerList.Back());
			Assert::AreEqual(2, FooPointerList.Length());

			FooPointerList.Clear(); 	
		}

		/// <summary>
		/// Test Clear Method on all lists
		/// </summary>
		TEST_METHOD(Clear)
		{
			int x = 4;
			//Test Clear on Int list	
			IntList.PushBack(x);
			IntList.Clear();
			Assert::AreEqual(0, IntList.Length());

			//Test Clear on IntPointer List
			int *temp = &x;

			IntPointerList.PushBack(temp);
			IntPointerList.Clear();
			Assert::AreEqual(0, IntPointerList.Length());

			//Test Clear on float list
			FloatList.PushBack(2.374f);
			FloatList.Clear();
			Assert::AreEqual(0, FloatList.Length());

			//Test Clear on String list
			std::string string1 = "Greetings";
			StringList.PushBack(string1);
			StringList.Clear();
			Assert::AreEqual(0, StringList.Length());

			//Test Clear on a Foo list
			Foo a;
			FooList.PushBack(a);
			FooList.Clear();
			Assert::AreEqual(0, FooList.Length());

			//Test Clear on a pointer to Foo List
			Foo *b = &a;
			FooPointerList.PushBack(b);
			FooPointerList.Clear();
			Assert::AreEqual(0, FooPointerList.Length());
		}

		/// <summary>
		/// Test PushFront method for all list
		/// </summary>
		TEST_METHOD(PushFront)
		{
			//Test PushFront for Int Lists
			int x = 5;
			int y = 7;
			IntList.PushFront(x);
			Assert::AreEqual(x, IntList.Front());
			Assert::AreEqual(x, IntList.Back());
			Assert::AreEqual(1, IntList.Length());

			IntList.PushFront(y);
			Assert::AreEqual(y, IntList.Front());
			Assert::AreEqual(x, IntList.Back());
			Assert::AreEqual(2, IntList.Length());
			
			//Test PushFront on Int* Lists
			int *Temp = &x;

			IntPointerList.PushFront(Temp);
			Assert::AreEqual(Temp, IntPointerList.Front());
			Assert::AreEqual(Temp, IntPointerList.Back());
			Assert::AreEqual(1, IntPointerList.Length());

			int *Temp2 = &y;

			IntPointerList.PushFront(Temp2);
			Assert::AreEqual(Temp2, IntPointerList.Front());
			Assert::AreEqual(Temp, IntPointerList.Back());
			Assert::AreEqual(2, IntPointerList.Length());

			//Test PushFront on float list
			float f1 = 7.0f;
			float f2 = 8.0f;

			FloatList.PushFront(f1);
			Assert::AreEqual(f1, FloatList.Front());
			Assert::AreEqual(f1, FloatList.Back());
			Assert::AreEqual(1, FloatList.Length());

			FloatList.PushFront(f2);
			Assert::AreEqual(f2, FloatList.Front());
			Assert::AreEqual(f1, FloatList.Back());
			Assert::AreEqual(2, FloatList.Length());

			//Test PushFront on String lists
			std::string Word = "Hello";
			StringList.PushFront(Word);
			Assert::AreEqual(Word, StringList.Front());
			Assert::AreEqual(Word, StringList.Back());
			Assert::AreEqual(1, StringList.Length());

			std::string Word2 = "World";
			StringList.PushFront(Word2);
			Assert::AreEqual(Word2, StringList.Front());
			Assert::AreEqual(Word, StringList.Back());
			Assert::AreEqual(2, StringList.Length());

			//Test PushFront on Foo List
			Foo a;
			FooList.PushFront(a);
			Assert::AreEqual(a, FooList.Front());
			Assert::AreEqual(a, FooList.Back());
			Assert::AreEqual(1, FooList.Length());

			Foo b(5);
			FooList.PushFront(b);
			Assert::AreEqual(b, FooList.Front());
			Assert::AreEqual(a, FooList.Back());
			Assert::AreEqual(2, FooList.Length());

			//Test PushFront on Foo* List
			Foo *c = &a;
			FooPointerList.PushFront(c);
			Assert::AreEqual(FooPointerList.Front(), c);

			Foo *d = &b;
			FooPointerList.PushFront(d);
			Assert::AreEqual(FooPointerList.Front(), d);
			Assert::AreEqual(FooPointerList.Back(), c);
			Assert::AreEqual(2, FooPointerList.Length());

			//Clear all Lists
			IntList.Clear();
			IntPointerList.Clear();
			FloatList.Clear();
			StringList.Clear();
			FooList.Clear();
			FooPointerList.Clear();
		}

		/// <summary>
		/// Test Copy Constructor for all lists
		/// </summary>
		TEST_METHOD(CopyConstructor)
		{
			//Test Copy Constructor for Int List
			int x = 2;
			int y = 4;
			IntList.PushBack(x);
			IntList.PushBack(y);

			SList<int> IntList2(IntList);

			Assert::AreEqual(IntList.Front(), IntList2.Front());
			Assert::AreEqual(IntList.Back(), IntList2.Back());
			Assert::AreEqual(IntList.Length(), IntList.Length());

			//Test Copy Constructor for Int* Lists
			int *Temp = &x;
			IntPointerList.PushFront(Temp);

			int *Temp2 = &y;
			IntPointerList.PushBack(Temp2);

			SList<int*> IntPointerList2(IntPointerList);

			Assert::AreEqual(IntPointerList.Front(), IntPointerList2.Front());
			Assert::AreEqual(IntPointerList.Back(), IntPointerList2.Back());
			Assert::AreEqual(IntPointerList.Length(), IntPointerList2.Length());

			//Test Copy Constructor for Float Lists
			float f1 = 2.5f, f2 = 4.5f, f3 = 6.8f;
			FloatList.PushBack(f1);
			FloatList.PushFront(f2);
			FloatList.PushBack(f3);

			SList<float> FloatList2(FloatList);

			Assert::AreEqual(FloatList.Front(), FloatList2.Front());
			Assert::AreEqual(FloatList.Back(), FloatList2.Back());
			Assert::AreEqual(FloatList.Length(), FloatList2.Length());

			//Test Copy Constructor for string list
			std::string string1 = "Hello", string2 = "Hollow", string3 = "World";
			StringList.PushBack(string1);
			StringList.PushBack(string2);
			StringList.PushBack(string3);

			SList<std::string> StringList2(StringList);

			Assert::AreEqual(StringList.Front(), StringList2.Front());
			Assert::AreEqual(StringList.Back(), StringList2.Back());
			Assert::AreEqual(StringList.Length(), StringList2.Length());

			//Test copy constructor for Foo list
			Foo foo1(4, 2), foo2(5, 8), foo3(6,9);
			FooList.PushBack(foo1);
			FooList.PushFront(foo2);
			FooList.PushBack(foo3);
			
			SList<Foo> FooList2(FooList);

			Assert::AreEqual(FooList.Front(), FooList2.Front());
			Assert::AreEqual(FooList.Back(), FooList2.Back());
			Assert::AreEqual(FooList.Length(), FooList2.Length());

			//Test copy constructor for Foo* list
			Foo *a = &foo1;
			FooPointerList.PushBack(a);

			Foo *b = &foo2;
			FooPointerList.PushBack(b);
				
			SList<Foo*> FooPointerList2(FooPointerList);

			Assert::AreEqual(FooPointerList.Front() , FooPointerList2.Front());
			Assert::AreEqual(FooList.Back() , FooList2.Back());
			Assert::AreEqual(FooList.Length(), FooList2.Length());

			//delete all heap data

			//Clear all lists
			IntList.Clear();
			IntPointerList.Clear();
			FloatList.Clear();
			StringList.Clear();
			FooList.Clear();
			FooPointerList.Clear();
		}

		/// <summary>
		/// Test Operator= for all lists
		/// </summary>
		TEST_METHOD(OperatorEqual)
		{
			//Test Operator= for Int lists
			int x = 2;
			int y = 4;
			IntList.PushBack(x);
			IntList.PushBack(y);

			SList<int> IntList2;
			IntList2 = IntList;

			Assert::AreEqual(IntList.Front(), IntList2.Front());
			Assert::AreEqual(IntList.Back(), IntList2.Back());
			Assert::AreEqual(IntList.Length(), IntList.Length());

			//Test Operator= for Int* lists
			int *Temp = &x;
			IntPointerList.PushFront(Temp);

			int *Temp2 = &y;
			IntPointerList.PushBack(Temp2);

			SList<int*> IntPointerList2;
			IntPointerList2 = IntPointerList;

			Assert::AreEqual(IntPointerList.Front(), IntPointerList2.Front());
			Assert::AreEqual(IntPointerList.Back(), IntPointerList2.Back());
			Assert::AreEqual(IntPointerList.Length(), IntPointerList2.Length());

			//Test operator= for float lists
			FloatList.PushBack(4);
			FloatList.PushFront(2);
			FloatList.PushBack(6);

			SList<float> FloatList2;
			FloatList2 = FloatList;

			Assert::AreEqual(FloatList.Front(), FloatList2.Front());
			Assert::AreEqual(FloatList.Back(), FloatList2.Back());
			Assert::AreEqual(FloatList.Length(), FloatList2.Length());

			//Test operator= for string list
			StringList.PushBack("Hello,");
			StringList.PushBack("Hollow");
			StringList.PushBack("World");

			SList<std::string> StringList2;
			StringList2 = StringList;

			Assert::AreEqual(StringList.Front(), StringList2.Front());
			Assert::AreEqual(StringList.Back(), StringList2.Back());
			Assert::AreEqual(StringList.Length(), StringList2.Length());

			//Test operator= for Foo list
			Foo foo1(4, 2), foo2(5, 8), foo3(6, 9);
			FooList.PushBack(foo1);
			FooList.PushFront(foo2);
			FooList.PushBack(foo3);

			SList<Foo> FooList2;
			FooList2 = FooList;

			Assert::AreEqual(FooList.Front(), FooList2.Front());
			Assert::AreEqual(FooList.Back(), FooList2.Back());
			Assert::AreEqual(FooList.Length(), FooList2.Length());

			//Test operator= for Foo* list
			Foo *a = &foo1;
			FooPointerList.PushBack(a);

			Foo *b = &foo2;
			FooPointerList.PushBack(b);

			SList<Foo*> FooPointerList2;
			FooPointerList2 = FooPointerList;

			Assert::AreEqual(FooPointerList.Front() , FooPointerList2.Front());
			Assert::AreEqual(FooList.Back() , FooList2.Back());
			Assert::AreEqual(FooList.Length() , FooList2.Length());

			//Clear all lists
			IntList.Clear();
			IntPointerList.Clear();
			FloatList.Clear();
			StringList.Clear();
			FooList.Clear();
			FooPointerList.Clear();
		}

		/// <summary>
		/// Test Pop front for all lists
		/// </summary>
		TEST_METHOD(PopFront)
		{
			//Test Pop front for Int lists
			int w = 3;
			int x = 5;
			int y = 4;
			int z = 9;
			IntList.PushBack(w);
			IntList.PushBack(x);
			IntList.PushBack(y);
			IntList.PushBack(z);

			Assert::AreEqual(w, IntList.Front());
			Assert::AreEqual(4, IntList.Length());
			IntList.PopFront();
			Assert::AreEqual(x, IntList.Front());
			Assert::AreEqual(3, IntList.Length());
			IntList.PopFront();
			Assert::AreEqual(y, IntList.Front());
			Assert::AreEqual(2, IntList.Length());
			IntList.PopFront();
			Assert::AreEqual(z, IntList.Front());
			Assert::AreEqual(1, IntList.Length());
			IntList.PopFront();
			Assert::AreEqual(0, IntList.Length());

			//Test Pop front for int* lists

			int *Temp = &x;
			IntPointerList.PushFront(Temp);

			int *Temp2 = &y;
			IntPointerList.PushBack(Temp2);

			Assert::AreEqual(Temp, IntPointerList.Front());
			Assert::AreEqual(2, IntPointerList.Length());
			IntPointerList.PopFront();
			Assert::AreEqual(Temp2, IntPointerList.Front());
			Assert::AreEqual(1, IntPointerList.Length());
			IntPointerList.PopFront();
			Assert::AreEqual(0, IntPointerList.Length());
			
			//Test Pop front for float list
			FloatList.PushBack(5.125f);
			FloatList.PushBack(6.125f);
			FloatList.PushBack(3.125f);
			FloatList.PushBack(1.125f);

			Assert::AreEqual(5.125f, FloatList.Front());
			Assert::AreEqual(4, FloatList.Length());
			FloatList.PopFront();
			Assert::AreEqual(6.125f, FloatList.Front());
			Assert::AreEqual(3, FloatList.Length());
			FloatList.PopFront();
			Assert::AreEqual(3.125f, FloatList.Front());
			Assert::AreEqual(2, FloatList.Length());
			FloatList.PopFront();
			Assert::AreEqual(1.125f, FloatList.Front());
			Assert::AreEqual(1, FloatList.Length());
			FloatList.PopFront();
			Assert::AreEqual(0, FloatList.Length());

			//Test Pop front for String list
			std::string Word = "Hello";

			StringList.PushBack("Hello");
			StringList.PushBack(",");
			StringList.PushBack("Hollow");
			StringList.PushBack("World");

			Assert::AreEqual(Word, StringList.Front());
			Assert::AreEqual(4, StringList.Length());
			Word = ",";
			StringList.PopFront();
			Assert::AreEqual(Word, StringList.Front());
			Assert::AreEqual(3, StringList.Length());
			Word = "Hollow";
			StringList.PopFront();
			Assert::AreEqual(Word, StringList.Front());
			Assert::AreEqual(2, StringList.Length());
			Word = "World";
			StringList.PopFront();
			Assert::AreEqual(Word, StringList.Front());
			Assert::AreEqual(1, StringList.Length());
			StringList.PopFront();
			Assert::AreEqual(0, StringList.Length());

			//Test Pop front for Foo list
			Foo A(5, 2), B(15, 33), C(16, 9), D(2, 55);
			
			FooList.PushBack(A);
			FooList.PushBack(B);
			FooList.PushBack(C);
			FooList.PushBack(D);

			Assert::AreEqual(A, FooList.Front());
			Assert::AreEqual(4, FooList.Length());
			FooList.PopFront();
			Assert::AreEqual(B, FooList.Front());
			Assert::AreEqual(3, FooList.Length());
			FooList.PopFront();
			Assert::AreEqual(C, FooList.Front());
			Assert::AreEqual(2, FooList.Length());
			FooList.PopFront();
			Assert::AreEqual(D, FooList.Front());
			Assert::AreEqual(1, FooList.Length());
			FooList.PopFront();
			Assert::AreEqual(0, FooList.Length());

			//Test Pop front for Foo* list
			Foo *E = &A;
			FooPointerList.PushBack(E);

			Foo *F = &B;
			FooPointerList.PushBack(F);

			FooPointerList.PopFront();
			Assert::AreEqual(F, FooPointerList.Front());
			Assert::AreEqual(1, FooPointerList.Length());

			FooPointerList.PopFront();
			Assert::AreEqual(0, FooPointerList.Length());

			//Clear Lists
			IntList.Clear();
			IntPointerList.Clear();
			FloatList.Clear();
			StringList.Clear();
			FooList.Clear();
			FooPointerList.Clear();
		}

		/// <summary>
		/// Test Front and Back for const references to Slist
		/// </summary>
		TEST_METHOD(ConstListTests)
		{
			//Const IntList test
			IntList.PushBack(4);
			const SList<int> &ConstIntList=IntList;
			Assert::AreEqual(ConstIntList.Front(), IntList.Front());
			Assert::AreEqual(ConstIntList.Back(), IntList.Back());

			//Const Int* tests
			int x = 4;
			int *Temp = &x;
			IntPointerList.PushFront(Temp);

			const SList<int *> &ConstIntPointerList = IntPointerList;
			Assert::AreEqual(ConstIntPointerList.Front(), IntPointerList.Front());
			Assert::AreEqual(ConstIntPointerList.Back(), IntPointerList.Back());


			//Const Float tests
			FloatList.PushBack(3.5f);
			const SList<float> &ConstFloatList = FloatList;
			Assert::AreEqual(ConstFloatList.Front(), FloatList.Front());
			Assert::AreEqual(ConstFloatList.Back(), FloatList.Back());

			//Const String tests
			StringList.PushBack("Hello, Hollow World!");
			const SList<std::string> &ConstStringList = StringList;
			Assert::AreEqual(ConstStringList.Front(), StringList.Front());
			Assert::AreEqual(ConstStringList.Back(), StringList.Back());
				
			//Const Foo tests
			Foo A;
			FooList.PushBack(A);
			const SList<Foo> &ConstFooList = FooList;
			Assert::AreEqual(ConstFooList.Front(), FooList.Front());
			Assert::AreEqual(ConstFooList.Back(), FooList.Back());

			//Const Foo* tests
			Foo *E = &A;
			FooPointerList.PushBack(E);
			const SList<Foo*> &ConstFooPointerList = FooPointerList;
			Assert::AreEqual(ConstFooPointerList.Front(), FooPointerList.Front());
			Assert::AreEqual(ConstFooPointerList.Back(), FooPointerList.Back());

			//Clear all lists
			IntList.Clear();
			IntPointerList.Clear();
			FloatList.Clear();
			StringList.Clear();
			FooList.Clear();
			FooPointerList.Clear();
		}

		/// <summary>
		/// Exception testing for calls to Front and Back when lists are empty
		/// </summary>
		TEST_METHOD(ExceptionTesting)
		{
			//IntList Exception testing
			SList<int> IntList2;
			std::function<void()> Expression = [&IntList2]() {IntList2.Front(); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&IntList2]() {IntList2.Back(); };
			Assert::ExpectException<std::exception>(Expression);

			const SList<int> ConstIntList2;
			Expression = [&ConstIntList2]() {ConstIntList2.Front(); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&ConstIntList2]() {ConstIntList2.Back(); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&IntList2]() {IntList2.PopFront(); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&IntList2]() {IntList2.PopBack(); };
			Assert::ExpectException<std::exception>(Expression);
			
			//Foo List exception testing
			SList<Foo> FooList2;
			Expression = [FooList2]() {FooList2.Front(); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [FooList2]() {FooList2.Back(); };
			Assert::ExpectException<std::exception>(Expression);
		}

		TEST_METHOD(IteratorInitialization)
		{
			int x = 6;
			int y = 4;

			IntList.PushBack(x);
			IntList.PushBack(y);

			SList<int>::Iterator It = IntList.begin();
			Assert::AreEqual(*It, x);

			SList<int>::Iterator It2 = It;
			Assert::IsTrue(It == It2);

			SList<int>::Iterator It3 = IntList.end();
			Assert::IsFalse(It == It3);
			It = IntList.end();
			Assert::IsTrue(It == It3);

			const SList<int>::Iterator ConstIt = IntList.begin();
			Assert::AreEqual(IntList.Front(), *(ConstIt));

			int *a = &x;
			int *b = &y;
			IntPointerList.PushBack(a);
			IntPointerList.PushBack(b);

			SList<int*>::Iterator It4 = IntPointerList.begin();
			Assert::AreEqual(*It4, a);
			
			SList<int*>::Iterator It5 = IntPointerList.end();
			Assert::IsFalse(It4 == It5);

			Foo A(1, 6);
			Foo B(2, 4);

			FooList.PushBack(A);
			FooList.PushBack(B);

			SList<Foo>::Iterator It6 = FooList.begin();
			Assert::AreEqual(*It6, A);

			SList<Foo>::Iterator It7 = FooList.end();
			Assert::IsFalse(It6 == It7);

			IntList.Clear();
			IntPointerList.Clear();
			FooList.Clear();
		}

		TEST_METHOD(RemoveElement)
		{
			int w = 3;
			int x = 4;
			int y = 5;
			int z = 6;
			IntList.PushBack(w);
			IntList.PushBack(x);
			IntList.PushBack(y);
			IntList.PushBack(z);


			Assert::AreEqual(false, IntList.Remove(8));
			Assert::AreEqual(true,IntList.Remove(y));
			Assert::AreEqual(3, IntList.Length());
			Assert::AreEqual(true, IntList.Remove(w));
			Assert::AreEqual(4, IntList.Front());
			Assert::AreEqual(true, IntList.Remove(z));
			Assert::AreEqual(IntList.Back(), IntList.Front());
			Assert::AreEqual(true, IntList.Remove(x));

			IntList.PushBack(w);
			IntList.PushBack(x);
			IntList.PushBack(y);
			IntList.PushBack(z);

			SList<int>::Iterator It = IntList.begin();
			IntList.Remove(It);
			Assert::AreEqual(x, IntList.Front());

			int *a = &x;
			int *b = &y;
			int *c = &z;
			IntPointerList.PushBack(a);
			IntPointerList.PushBack(b);

			Assert::AreEqual(true, IntPointerList.Remove(a));
			Assert::AreEqual(false, IntPointerList.Remove(c));
	
			Foo A(1, 6);
			Foo B(2, 4);
			Foo C(3, 9);

			FooList.PushBack(A);
			FooList.PushBack(B);

			Assert::AreEqual(true, FooList.Remove(A));
			Assert::AreEqual(1, FooList.Length());
			Assert::AreEqual(false, FooList.Remove(C));
			Assert::AreEqual(1, FooList.Length());
			Assert::AreEqual(B, FooList.Front());

			IntList.Clear();
			IntPointerList.Clear();
			FooList.Clear();
		}

		TEST_METHOD(FindTest)
		{
			int w = 6;
			int x = 4;
			int y = 7;
			int z = 8;
			IntList.PushBack(w);
			IntList.PushBack(x);
			IntList.PushBack(y);
			IntList.PushBack(z);
			SList<int>::Iterator It = IntList.Find(x);

			Assert::AreEqual(x, *It);
			Assert::AreNotEqual(y, *It);

			
			int *a = &x;
			int *b = &y;
			IntPointerList.PushBack(a);
			IntPointerList.PushBack(b);

			SList<int*>::Iterator It2 = IntPointerList.Find(b);

			Assert::AreEqual(b, *It2);
			Assert::AreNotEqual(a, *It2);

			Foo A(1, 6);
			Foo B(2, 4);

			FooList.PushBack(A);
			FooList.PushBack(B);

			SList<Foo>::Iterator It3 = FooList.Find(B);

			Assert::AreEqual(B, *It3);
			Assert::AreNotEqual(A, *It3);

			IntList.Clear();
			IntPointerList.Clear();
			FooList.Clear();
		}

		TEST_METHOD(InsertAfter)
		{
			int x = 6;
			int y = 4;
			int z = 5;
			int w = 7;
			int u = 9;

			IntList.PushBack(x);

			SList<int>::Iterator It = IntList.end();
			IntList.InsertAfter(It, y);
			Assert::AreEqual(y, IntList.Back());

			IntList.PushBack(x);
			IntList.PushBack(y);
			IntList.PushBack(z);

			It = IntList.Find(z);
			IntList.InsertAfter(It, w);
			++It;
			Assert::AreEqual(w, *It);
			It = IntList.end();
			IntList.InsertAfter(It, u);
			Assert::AreEqual(IntList.Back(), u);
			


			int *a = &x;
			int *b = &y;
			IntPointerList.PushBack(a);

			SList<int*>::Iterator It2 = IntPointerList.end();
			IntPointerList.InsertAfter(It2,b);
			Assert::AreEqual(b, IntPointerList.Back());
			Assert::AreEqual(2, IntPointerList.Length());
			
			It2 = IntPointerList.begin();
			int *c = &z;
			IntPointerList.InsertAfter(It2, c);
			Assert::AreEqual(c, *(++It2));
			Assert::AreEqual(3, IntPointerList.Length());

			Foo A(1, 1), B(2, 2), C(4,3);

			FooList.PushBack(A);

			SList<Foo>::Iterator It3 = FooList.end();
			FooList.InsertAfter(It3, B);
			Assert::AreEqual(B, FooList.Back());
			
			It3 = FooList.begin();
			FooList.InsertAfter(It3, C);
			Assert::AreEqual(C, *(++It3));

			IntList.Clear();
			IntPointerList.Clear();
			FooList.Clear();

		}

		TEST_METHOD(PopBack)
		{
			int w = 3;
			int x = 4;
			int y = 5;
			int z = 6;

			IntList.PushBack(w);
			IntList.PushBack(x);
			IntList.PushBack(y);
			IntList.PushBack(z);
			Assert::AreEqual(z, IntList.Back());
			IntList.PopBack();
			Assert::AreEqual(y, IntList.Back());
			IntList.PopBack();
			Assert::AreEqual(x, IntList.Back());
			IntList.PopBack();
			Assert::AreEqual(w, IntList.Back());
			IntList.PopBack();


			int *a = &x;
			int *b = &y;
			IntPointerList.PushBack(a);
			IntPointerList.PushBack(b);

			Assert::AreEqual(b, IntPointerList.Back());
			IntPointerList.PopBack();
			Assert::AreEqual(a, IntPointerList.Back());

			Foo fooA(3, 4), fooB(5, 8), fooC(6, 3);

			FooList.PushBack(fooA);
			FooList.PushBack(fooB);
			FooList.PushBack(fooC);

			Assert::AreEqual(fooC, FooList.Back());
			FooList.PopBack();
			Assert::AreEqual(fooB, FooList.Back());
			FooList.PopBack();
			Assert::AreEqual(fooA, FooList.Back());

			IntList.Clear();
			IntPointerList.Clear();
			FooList.Clear();
		}

		TEST_METHOD(RemoveAll)
		{
			int x = 6;
			int y = 4;

			IntList.PushBack(x);
			IntList.PushBack(x);
			IntList.PushBack(x);
			IntList.PushBack(y);
			IntList.PushBack(y);
			IntList.PushBack(x);
			IntList.PushBack(x);
			IntList.PushBack(x);
			IntList.PushBack(x);
			IntList.PushBack(y);
			IntList.PushBack(x);
			IntList.PushBack(x);
			
			IntList.RemoveAll(x);
			Assert::AreEqual(3, IntList.Length());

			int *a = &x;
			int *b = &y;
			IntPointerList.PushBack(a);
			IntPointerList.PushBack(b);
			IntPointerList.PushBack(a);
			IntPointerList.PushBack(b);
			IntPointerList.PushBack(a);
			IntPointerList.PushBack(b);
			IntPointerList.PushBack(a);
			IntPointerList.PushBack(b);

			IntPointerList.RemoveAll(a);
			Assert::AreEqual(4, IntPointerList.Length());
			IntPointerList.RemoveAll(b);
			Assert::AreEqual(true, IntPointerList.IsEmpty());

			Foo fooA(3, 4), fooB(5, 8), fooC(6, 3);

			FooList.PushBack(fooA);
			FooList.PushBack(fooB);
			FooList.PushBack(fooC);
			FooList.PushBack(fooA);
			FooList.PushBack(fooB);
			FooList.PushBack(fooA);
			FooList.PushBack(fooB);
			FooList.PushBack(fooA);
			FooList.PushBack(fooA);
			FooList.PushBack(fooB);
			FooList.PushBack(fooB);

			FooList.RemoveAll(fooA);
			FooList.RemoveAll(fooB);
			Assert::AreEqual(1, FooList.Length());
			Assert::AreEqual(fooC, *(FooList.begin()));

			IntList.Clear();
			IntPointerList.Clear();
			FooList.Clear();
		}

		TEST_METHOD(OperatorEquality)
		{
			IntList.PushBack(4);
			IntList.PushBack(5);

			SList<int> IntList2 = IntList;
			SList<int>::Iterator It = IntList.begin(), It2 = IntList2.begin();
			Assert::IsFalse(It == It2);

			It2 = IntList.begin();
			Assert::IsTrue(It == It2);

			It = IntList.end();
			It2 = IntList2.end();
			Assert::IsFalse(It == It2);

			It2 = IntList.end();
			Assert::IsTrue(It == It2);

			int a = 6;
			int b = 4;
			IntPointerList.PushBack(&a);
			IntPointerList.PushBack(&b);
			SList<int*> IntPointerList2 = IntPointerList;
			SList<int*>::Iterator It3 = IntPointerList.begin(), It4 = IntPointerList2.begin();
			Assert::IsFalse(It3 == It4);
			It3 = IntPointerList.end();
			Assert::IsFalse(It3 == It4);
			
			It3 = IntPointerList2.begin();
			Assert::IsTrue(It3 == It4);

			Foo foo1(3, 4), foo2(5, 8), foo3(6, 3);

			FooList.PushBack(foo1);
			FooList.PushBack(foo2);
			FooList.PushBack(foo3);
			SList<Foo> FooList2 = FooList;
			SList<Foo>::Iterator It5 = FooList.begin(), It6 = FooList2.begin();
			Assert::IsFalse(It5 == It6);
			It6 = FooList.end();
			Assert::IsFalse(It5 == It6);

			It6 = FooList.begin();
			Assert::IsTrue(It5 == It6);

			IntList.Clear();
			IntList2.Clear();
			IntPointerList.Clear();
			IntPointerList2.Clear();
			FooList.Clear();
			FooList2.Clear();
		}

		TEST_METHOD(OperatorInequality)
		{
			IntList.PushBack(4);
			IntList.PushBack(5);

			SList<int> IntList2 = IntList;
			SList<int>::Iterator It = IntList.begin(), It2 = IntList2.begin();
			Assert::IsTrue(It != It2);

			It2 = IntList.begin();
			Assert::IsFalse(It != It2);

			It = IntList.end();
			It2 = IntList2.end();
			Assert::IsTrue(It != It2);

			It2 = IntList.end();
			Assert::IsFalse(It != It2);

			int a = 6;
			int b = 4;
			IntPointerList.PushBack(&a);
			IntPointerList.PushBack(&b);

			SList<int*> IntPointerList2 = IntPointerList;
			SList<int*>::Iterator It3 = IntPointerList.begin(), It4 = IntPointerList2.begin();
			Assert::IsTrue(It3 != It4);
			It3 = IntPointerList.end();
			Assert::IsTrue(It3 != It4);

			It3 = IntPointerList2.begin();
			Assert::IsFalse(It3 != It4);

			Foo A(3, 4), B(5, 8), C(6, 3);

			FooList.PushBack(A);
			FooList.PushBack(B);
			FooList.PushBack(C);

			SList<Foo> FooList2 = FooList;
			SList<Foo>::Iterator It5 = FooList.begin(), It6 = FooList2.begin();
			Assert::IsTrue(It5 != It6);
			It6 = FooList.end();
			Assert::IsTrue(It5 != It6);

			It6 = FooList.begin();
			Assert::IsFalse(It5 != It6);

			IntList.Clear();
			IntList2.Clear();
			IntPointerList.Clear();
			IntPointerList2.Clear();
			FooList.Clear();
			FooList2.Clear();
		}

		TEST_METHOD(OperatorIncrement)
		{
			int x = 4;
			IntList.PushBack(x++);
			IntList.PushBack(x++);
			IntList.PushBack(x++);
			IntList.PushBack(x++);

			x = 4;
			int y = 5;

			//Prefix
			SList<int>::Iterator It = IntList.begin();
			for (; It != IntList.end(); ++It)
			{
				Assert::AreEqual(x,*(It));
				x++;
			}

			//Postfix
			x = 4;
			for (It=IntList.begin(); It != IntList.end(); It++)
			{
				Assert::AreEqual(x, *(It));
				x++;
			}

			IntPointerList.PushBack(&x);
			IntPointerList.PushBack(&y);
			
			SList<int*>::Iterator It2 = IntPointerList.begin();
			++It2;
			Assert::AreEqual(&y, *It2);
			It2 = IntPointerList.begin();
			Assert::AreEqual(&x, *(It2++));
			Assert::AreEqual(&y, *It2);

			Foo A(3, 4), B(5, 8), C(6, 3);

			FooList.PushBack(A);
			FooList.PushBack(B);
			FooList.PushBack(C);
			
			SList<Foo>::Iterator It3 = FooList.begin();
			++It3;
			Assert::AreEqual(B, *It3);
			Assert::AreEqual(B, *(It3++));
			Assert::AreEqual(C, *It3);

			IntList.Clear();
			IntPointerList.Clear();
			FooList.Clear();
		}

		TEST_METHOD(IteratorExceptionTesting)
		{
			int x = 8;
			IntList.PushBack(x);

			SList<int> IntList2;
			//const SList<int>& IntList3 = IntList2;
			SList<int>::Iterator It;
			std::function<void()> Expression;// = [&IntList2]() {IntList2.begin(); };
			/*Assert::ExpectException<std::exception>(Expression);

			Expression = [&IntList2]() {IntList2.end(); };
			Assert::ExpectException<std::exception>(Expression);*/

			Expression = [&IntList2, &It]() {IntList2.InsertAfter(It, 7); };
			Assert::ExpectException<std::exception>(Expression);

			/*Expression = [&IntList3]() {IntList3.begin(); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&IntList3]() {IntList3.end(); };
			Assert::ExpectException<std::exception>(Expression);*/

			Expression = [&It]() {It++; };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&It]() {++It; };
			Assert::ExpectException<std::exception>(Expression);

			SList<int *> IntPointerList2;
			SList<int *>::Iterator It2;
			/*Expression = [&IntPointerList2]() {IntPointerList2.begin(); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&IntPointerList2]() {IntPointerList2.end(); };
			Assert::ExpectException<std::exception>(Expression);*/

			Expression = [&It2]() {It2++; };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&It2]() {++It2; };
			Assert::ExpectException<std::exception>(Expression);

			It = IntList.begin();
			Expression = [&IntList2, &It]() {IntList2.InsertAfter(It, 7); };
			Assert::ExpectException<std::exception>(Expression);

			SList<Foo> FooList2;
			SList<Foo>::Iterator It3;

		/*	Expression = [&FooList2]() {FooList2.begin(); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&FooList2]() {FooList2.begin(); };
			Assert::ExpectException<std::exception>(Expression);*/

			Expression = [&It3]() {It3++; };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&It3]() {++It3; };
			Assert::ExpectException<std::exception>(Expression);



			IntList.Clear();
			IntList2.Clear();
			IntPointerList.Clear();
			FooList.Clear();
		}
	};

	_CrtMemState SListTest::sStartMemState;
}