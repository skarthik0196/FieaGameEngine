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

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<UnitTestLibraryDesktop::Foo>(const UnitTestLibraryDesktop::Foo& T)
	{
		RETURN_WIDE_STRING(T.GetData());
	}

	/*template<>
	std::wstring ToString<UnitTestLibraryDesktop::Foo>(UnitTestLibraryDesktop::Foo* T)
	{
		RETURN_WIDE_STRING(T->GetData());
	}*/

}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(VectorTest)
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

		TEST_METHOD(VectorInitializationTest)
		{	
			Vector<int> IntVector;
			Assert::AreEqual(true, IntVector.IsEmpty());
			Assert::AreEqual(0U, IntVector.Length());

			Vector<int*> IntPointerVector;
			Assert::AreEqual(true, IntPointerVector.IsEmpty());
			Assert::AreEqual(0U, IntPointerVector.Length());

			Vector<Foo> FooVector;
			Assert::AreEqual(true, FooVector.IsEmpty());
			Assert::AreEqual(0U, FooVector.Length());
		}

		TEST_METHOD(OperatorDereferencingTest)
		{
			int x = 2;
			int y = 3;
			int z = 6;

			Vector<int> IntVector;
			IntVector.PushBack(x);
			IntVector.PushBack(y);
			IntVector.PushBack(z);

			Assert::AreEqual(x, IntVector[0]);
			Assert::AreEqual(y, IntVector[1]);
			Assert::AreEqual(z, IntVector[2]);

			const Vector<int>& ConstIntVector = IntVector;

			Assert::AreEqual(x, ConstIntVector[0]);
			Assert::AreEqual(y, ConstIntVector[1]);
			Assert::AreEqual(z, ConstIntVector[2]);

			Foo a(5, 6), b(5,4);

			Vector<Foo> FooVector;
			FooVector.PushBack(a);
			FooVector.PushBack(b);

			Assert::AreEqual(a, FooVector[0]);
			Assert::AreEqual(b, FooVector[1]);
		}

		TEST_METHOD(PushBack)
		{
			Vector<int> IntVector;

			int x = 3;
			int y = 4;

			IntVector.PushBack(x);
			Assert::AreEqual(1U, IntVector.Length());
			Assert::AreEqual(x, IntVector[0]);

			IntVector.PushBack(y);
			Assert::AreEqual(2U, IntVector.Length());
			Assert::AreEqual(y, IntVector[1]);


			int *a = &x;
			int *b = &y;

			Vector<int*> IntPointerVector;
			IntPointerVector.PushBack(a);
			IntPointerVector.PushBack(b);

			Assert::AreEqual(2U, IntPointerVector.Length());
			Assert::AreEqual(a, IntPointerVector[0]);
			Assert::AreEqual(b, IntPointerVector[1]);

			Foo c(4, 8), d(1, 5);
			Vector<Foo> FooVector;

			FooVector.PushBack(c);
			FooVector.PushBack(d);

			Assert::AreEqual(2U, FooVector.Length());
			Assert::AreEqual(c, FooVector[0]);
			Assert::AreEqual(d, FooVector[1]);
		}

		TEST_METHOD(PopBack)
		{
			int x = 2;
			int y = 3;
			int z = 6;

			Vector<int> IntVector;
			IntVector.PushBack(x);
			IntVector.PushBack(y);
			IntVector.PushBack(z);
			IntVector.PushBack(y);
			IntVector.PushBack(z);
			IntVector.PushBack(x);

			Assert::AreEqual(6U, IntVector.Length());
			Assert::AreEqual(x, IntVector[5]);
			IntVector.PopBack();
			Assert::AreEqual(5U, IntVector.Length());
			Assert::AreEqual(z, IntVector[4]);
			IntVector.PopBack();
			Assert::AreEqual(4U, IntVector.Length());
			Assert::AreEqual(y, IntVector[3]);
			IntVector.PopBack();
			Assert::AreEqual(3U, IntVector.Length());
			Assert::AreEqual(z, IntVector[2]);
			IntVector.PopBack();
			Assert::AreEqual(2U, IntVector.Length());
			Assert::AreEqual(y, IntVector[1]);
			IntVector.PopBack();
			Assert::AreEqual(1U, IntVector.Length());
			Assert::AreEqual(x, IntVector[0]);
			IntVector.PopBack();

			int *a = &x;
			int *b = &y;

			Vector<int*> IntPointerVector;
			IntPointerVector.PushBack(a);
			IntPointerVector.PushBack(b);

			Assert::AreEqual(2U, IntPointerVector.Length());
			Assert::AreEqual(b, IntPointerVector[1]);
			IntPointerVector.PopBack();
			Assert::AreEqual(1U, IntPointerVector.Length());
			Assert::AreEqual(a, IntPointerVector[0]);
			IntPointerVector.PopBack();
			Assert::AreEqual(true, IntPointerVector.IsEmpty());

			Foo c(4, 8), d(1, 5);
			Vector<Foo> FooVector;

			FooVector.PushBack(c);
			FooVector.PushBack(d);

			Assert::AreEqual(2U, FooVector.Length());
			Assert::AreEqual(d, FooVector[1]);
			FooVector.PopBack();
			Assert::AreEqual(c, FooVector[0]);
			Assert::AreEqual(1U, FooVector.Length());
			FooVector.PopBack();
			Assert::AreEqual(true, FooVector.IsEmpty());
		}

		TEST_METHOD(ReserveTesting)
		{
			int x = 3;
			int y = 4;

			Vector<int> IntVector2;
			IntVector2.Reserve(10);
			IntVector2.PushBack(x);
			IntVector2.PushBack(x);
			IntVector2.PushBack(x);
			IntVector2.PushBack(x);
			Assert::AreEqual(4U, IntVector2.Length());
			Assert::AreEqual(10U, IntVector2.CurrentCapacity());

			int *a = &x;
			int *b = &y;

			Vector<int*> IntPointerVector;
			IntPointerVector.PushBack(a);
			IntPointerVector.PushBack(b);
			IntPointerVector.PushBack(a);
			Assert::AreEqual(4U, IntPointerVector.CurrentCapacity());
			IntPointerVector.Reserve(20);
			Assert::AreEqual(20U, IntPointerVector.CurrentCapacity());

			Foo c(4, 8), d(1, 5);
			Vector<Foo> FooVector;

			FooVector.PushBack(c);
			FooVector.PushBack(d);
			Assert::AreEqual(2U, FooVector.CurrentCapacity());
			FooVector.Reserve(15);
			Assert::AreEqual(15U, FooVector.CurrentCapacity());

		}

		TEST_METHOD(FrontAndBack)
		{
			int x[] = { 3,62,463,2,6,75,4 };

			Vector<int> IntVector;
			for (int i = 0; i < 7; i++)
			{
				IntVector.PushBack(x[i]);
			}

			Assert::AreEqual(x[0], IntVector.Front());
			Assert::AreEqual(x[6], IntVector.Back());

			IntVector.Front() = x[2];
			IntVector.Back() = x[2];

			Assert::AreEqual(x[2], IntVector.Front());
			Assert::AreEqual(x[2], IntVector.Back());

			const Vector<int>& ConstIntVector = IntVector;
			Assert::AreEqual(x[2], ConstIntVector.Front());
			Assert::AreEqual(x[2], ConstIntVector.Back());

			int *a = &x[0];
			int *b = &x[1];

			Vector<int*> IntPointerVector;
			IntPointerVector.PushBack(a);
			IntPointerVector.PushBack(b);
			Assert::AreEqual(a, IntPointerVector.Front());
			Assert::AreEqual(b, IntPointerVector.Back());

			Foo c(4, 8), d(1, 5);
			Vector<Foo> FooVector;

			FooVector.PushBack(c);
			FooVector.PushBack(d);

			Assert::AreEqual(c, FooVector.Front());
			Assert::AreEqual(d, FooVector.Back());
		}

		TEST_METHOD(Shrink)
		{
			int x[] = { 3,62,463,2,6,75,4 };

			Vector<int> IntVector;
			for (int i = 0; i < 7; i++)
			{
				IntVector.PushBack(x[i]);
			}

			Assert::AreEqual(8U, IntVector.CurrentCapacity());
			IntVector.Shrink();
			Assert::AreEqual(7U, IntVector.CurrentCapacity());

			int *a = &x[0];
			int *b = &x[1];

			Vector<int*> IntPointerVector;
			IntPointerVector.PushBack(a);
			IntPointerVector.PushBack(b);
			IntPointerVector.PushBack(a);
			Assert::AreEqual(4U, IntPointerVector.CurrentCapacity());
			IntPointerVector.Shrink();
			Assert::AreEqual(3U, IntPointerVector.CurrentCapacity());

			Foo c(4, 8), d(1, 5);
			Vector<Foo> FooVector;

			FooVector.PushBack(c);
			FooVector.PushBack(d);
			FooVector.PushBack(d);
			Assert::AreEqual(4U, FooVector.CurrentCapacity());
			FooVector.Shrink();
			Assert::AreEqual(3U, FooVector.CurrentCapacity());
		}

		TEST_METHOD(IteratorInitialization)
		{
			int x = 4;
			int y = 7;
			Vector<int> IntVector;

			IntVector.PushBack(x);
			IntVector.PushBack(y);
			Assert::AreEqual(2U, IntVector.Length());

			Vector<int>::Iterator It=IntVector.begin();
			Assert::AreEqual(x, *It);
			*It = 7;
			Assert::AreEqual(IntVector[0], *It);

			const Vector<int>::Iterator ConstIterator = IntVector.begin();
			Assert::AreEqual(IntVector[0], *ConstIterator);

			Foo c(4, 8), d(1, 5);
			Vector<Foo> FooVector;

			FooVector.PushBack(c);
			FooVector.PushBack(d);
			Vector<Foo>::Iterator It2 = FooVector.begin();
			Assert::AreEqual(c, *It2);
			It2++;
			Assert::AreEqual(d, *It2);
		}

		TEST_METHOD(FindTesting)
		{
			int x[] = { 3,62,463,2,6,75,4 };

			Vector<int> IntVector;
			for (int i = 0; i < 7; i++)
			{
				IntVector.PushBack(x[i]);
			}

			Vector<int>::Iterator It = IntVector.Find(75);
			Assert::AreEqual(75, *It);
			It = IntVector.Find(463);
			Assert::AreEqual(463, *It);
			It = IntVector.Find(62);
			Assert::AreEqual(62, *It);

			int *a = &x[0];
			int *b = &x[1];

			Vector<int*> IntPointerVector;
			IntPointerVector.PushBack(a);
			IntPointerVector.PushBack(b);

			Vector<int*>::Iterator It2 = IntPointerVector.Find(b);
			Assert::AreEqual(b, *It2);

			Foo c(4, 8), d(1, 5);
			Vector<Foo> FooVector;

			FooVector.PushBack(c);
			FooVector.PushBack(d);

			Vector<Foo>::Iterator It3 = FooVector.Find(d);
			Assert::AreEqual(d, *It3);
		}
		
		TEST_METHOD(At)
		{
			int x[] = { 3,62,463,2,6,75,4 };

			Vector<int> IntVector;
			for (int i = 0; i < 7; i++)
			{
				IntVector.PushBack(x[i]);
			}

			const Vector<int>& ConstIntVector = IntVector;

			for (int i = 0; i < 7; i++)
			{
				Assert::AreEqual(x[i], IntVector.At(i));
				Assert::AreEqual(x[i], ConstIntVector.At(i));
			}

			int *a = &x[0];
			int *b = &x[1];

			Vector<int*> IntPointerVector;
			IntPointerVector.PushBack(a);
			IntPointerVector.PushBack(b);
			Assert::AreEqual(a, IntPointerVector.At(0));

			Foo c(4, 8), d(1, 5);
			Vector<Foo> FooVector;

			FooVector.PushBack(c);
			FooVector.PushBack(d);
			Assert::AreEqual(d, FooVector.At(1));
			
		}

		TEST_METHOD(CopyConstructorAndAssignment)
		{
			int x[] = { 3,62,463,2,6,75,4 };

			Vector<int> IntVector;
			for (int i = 0; i < 7; i++)
			{
				IntVector.PushBack(x[i]);
			}

			Vector<int> IntVector2(IntVector);

			for (int i = 0; i < 7; i++)
			{
				Assert::AreEqual(x[i], IntVector2.At(i));
				Assert::AreNotEqual(&IntVector.At(i), &IntVector2.At(i));
			}

			Vector<int> IntVector3;

			IntVector3 = IntVector2;

			for (int i = 0; i < 7; i++)
			{
				Assert::AreEqual(x[i], IntVector3.At(i));
				Assert::AreNotEqual(&IntVector2.At(i), &IntVector3.At(i));
			}

			Foo A(3, 4), B(6, 3), C(5, 7);
			Vector<Foo> FooVector;

			FooVector.PushBack(A);
			FooVector.PushBack(B);
			FooVector.PushBack(C);

			Vector<Foo> FooVector2(FooVector);

			Assert::AreEqual(A, FooVector2[0]);
			Assert::AreEqual(FooVector[0], FooVector2[0]);
			Assert::IsFalse(&FooVector[0] == &FooVector2[0]);

			Vector<Foo> FooVector3;
			FooVector3 = FooVector2;
			Assert::AreEqual(A, FooVector3[0]);
			Assert::AreEqual(B, FooVector3[1]);
			Assert::AreEqual(FooVector2[0], FooVector3[0]);
			Assert::IsFalse(&FooVector2[0] == &FooVector3[0]);
		}

		TEST_METHOD(Remove)
		{
			int x[] = { 3,62,463,2,6,75,4 };
			int y[] = { 3,62,463,6,75,4 };

			Vector<int> IntVector;
			for (int i = 0; i < 7; i++)
			{
				IntVector.PushBack(x[i]);
			}

			IntVector.Remove(3);
			Assert::AreEqual(6U, IntVector.Length());
			for (int i = 0; i < 6; i++)
			{
				Assert::AreEqual(y[i], IntVector.At(i));
			}
			IntVector.Remove(5);

			Foo A(3, 4), B(6, 3), C(5, 7);
			Vector<Foo> FooVector;

			FooVector.PushBack(A);
			FooVector.PushBack(B);
			FooVector.PushBack(C);

			FooVector.Remove(0);
			Assert::AreEqual(B, FooVector.Front());
			Assert::AreEqual(C, FooVector.Back());
			FooVector.Remove(1);
			Assert::AreEqual(B, FooVector.Back());
			Assert::AreEqual(1U, FooVector.Length());
		}

		TEST_METHOD(RemoveAll)
		{
			int x[] = { 3,62,463,2,6,75,4 };
			int y[] = { 3,6,75,4 };

			Vector<int> IntVector;
			for (int i = 0; i < 7; i++)
			{
				IntVector.PushBack(x[i]);
			}
			Vector<int>::Iterator It1 = IntVector.Find(62), It2 = IntVector.Find(2);
			IntVector.Remove(It1, It2);

			Assert::AreEqual(4U, IntVector.Length());
			for (int i = 0; i < 3; i++)
			{
				Assert::AreEqual(y[i], IntVector.At(i));
			}

			Foo A(3, 4), B(6, 3), C(5, 7), D(4, 6);
			Vector<Foo> FooVector;

			FooVector.PushBack(A);
			FooVector.PushBack(B);
			FooVector.PushBack(C);
			FooVector.PushBack(D);
			FooVector.PushBack(C);

			Vector<Foo>::Iterator It3 = FooVector.Find(B), It4 = FooVector.Find(D);

			FooVector.Remove(It3, It4);
			Assert::AreEqual(2U, FooVector.Length());
			Assert::AreEqual(A, FooVector.Front());
			Assert::AreEqual(C, FooVector.Back());
		}

		TEST_METHOD(IteratorEqualityTesting)
		{
			int x[] = { 3,62,463,2,6,75,4 };

			Vector<int> IntVector;
			for (int i = 0; i < 7; i++)
			{
				IntVector.PushBack(x[i]);
			}

			Vector<int> IntVector2(IntVector);
			Vector<int>::Iterator It1 = IntVector.begin(), It2 = IntVector2.begin();

			Assert::IsFalse(It1 == It2);
			Assert::IsTrue(It1 != It2);
			It2 = IntVector.end();
			Assert::IsFalse(It1 == It2);
			It2 = IntVector.begin();
			Assert::IsTrue(It1 == It2);
		}

		TEST_METHOD(ExceptionTesting)
		{

			Vector<int> IntVector;
			const Vector<int> IntVector2(IntVector);
			Vector<int>::Iterator It, It2;

			std::function<void()> Expression = [&IntVector]() {IntVector.Front(); };
			Assert::ExpectException<std::exception>(Expression);

			Expression= [&IntVector]() {IntVector.Back(); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&IntVector2]() {IntVector2.Front(); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&IntVector2]() {IntVector2.Back(); };
			Assert::ExpectException<std::exception>(Expression);

			//Expression = [&IntVector]() {IntVector.begin(); };
			//Assert::ExpectException<std::exception>(Expression);

			//Expression = [&IntVector]() {IntVector.end(); };
			//Assert::ExpectException<std::exception>(Expression);

			//Expression = [&IntVector2]() {IntVector2.begin(); };
			//Assert::ExpectException<std::exception>(Expression);

			//Expression = [&IntVector2]() {IntVector2.end(); };
			//Assert::ExpectException<std::exception>(Expression);

			Expression = [&It]() {*It; };
			Assert::ExpectException<std::exception>(Expression);

			int x[] = { 3,62,463,2,6,75,4 };
			for (int i = 0; i < 7; i++)
			{
				IntVector.PushBack(x[i]);
			}

			Expression = [&IntVector]() {IntVector[10]; };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&IntVector]() {IntVector.At(8); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&IntVector2]() {IntVector2[10]; };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&IntVector2]() {IntVector2.At(8); };
			Assert::ExpectException<std::exception>(Expression);

			Expression = [&IntVector]() {IntVector.Remove(11); };
			Assert::ExpectException<std::exception>(Expression);

			It = IntVector.Find(62);
			It2 = IntVector.Find(75);

			Expression = [&IntVector, &It, &It2]() {IntVector.Remove(It2, It); };
			Assert::ExpectException<std::exception>(Expression);

			const Vector<int>::Iterator ConstIterator;
			Expression = [&ConstIterator]() {*ConstIterator; };
			Assert::ExpectException<std::exception>(Expression);

		}

		TEST_METHOD(UserIncrementStrategy)
		{
			int x = 2;
			int y = 3;
			int z = 6;

			Vector<int, IntVectorIncrementStrategy> IntVector;
			IntVector.PushBack(x);
			IntVector.PushBack(y);

			Assert::AreEqual(3U, IntVector.CurrentCapacity());

			IntVector.PushBack(z);
			IntVector.PushBack(z);

			Assert::AreEqual(9U, IntVector.CurrentCapacity());

			Vector<Foo, IntVectorIncrementStrategy> FooVector;
			Foo a(4, 6), b(5, 7);
			FooVector.PushBack(a);
			FooVector.PushBack(b);
			
			Assert::AreEqual(3U, FooVector.CurrentCapacity());

			FooVector.PushBack(a);
			FooVector.PushBack(b);

			Assert::AreEqual(9U, FooVector.CurrentCapacity());
		}

		TEST_METHOD(VectorOfVectorTest)
		{
			int x = 2;
			int y = 3;
			int z = 6;

			Vector<int> IntVector;

			IntVector.PushBack(x);
			IntVector.PushBack(y);
			IntVector.PushBack(z);

			Vector<Vector<int>> VectorOfVector;

			VectorOfVector.PushBack(IntVector);
			VectorOfVector.PushBack(IntVector);
			VectorOfVector.PushBack(IntVector);

			Assert::AreEqual(x, VectorOfVector[0].Front());
			Assert::AreEqual(z, VectorOfVector[2].Back());
		}

		TEST_METHOD(Resize)
		{
			Vector<Foo> FooVector;
			Foo a(4, 60);

			FooVector.Resize(7);
			FooVector[6] = a;
			Assert::AreEqual(a, FooVector[6]);
			FooVector.Resize(3);
			FooVector[1] = a;
			Assert::AreEqual(a, FooVector[1]);

		}

		TEST_METHOD(MoveSemantics)
		{
			int x = 2;
			int y = 3;
			int z = 6;

			Vector<int> IntVector;

			IntVector.PushBack(x);
			IntVector.PushBack(y);
			IntVector.PushBack(z);

			Vector<int> IntVector2 = std::move(IntVector);

			Assert::AreEqual(IntVector2[2], z);
			Assert::AreEqual(0U, IntVector.Length());

			Vector<int> IntVector3;
			IntVector3 = std::move(IntVector2);

			Assert::AreEqual(IntVector3[2], z);
			Assert::AreEqual(0U, IntVector2.Length());
		}
	};

	_CrtMemState VectorTest::sStartMemState;
}