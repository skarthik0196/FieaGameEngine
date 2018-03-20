#pragma once
#include "pch.h"
#include "Foo.h"
#include<string>

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(Foo)
	RTTI_DEFINITIONS(Foo::FooSharedData)

	Foo::Foo(int Value, int Value2) : TestMember(Value), TestMember2(new int(Value2))
	{

	}

	Foo::Foo(const Foo & OtherFoo) : TestMember(0), TestMember2(new int(0))
	{
		TestMember = OtherFoo.TestMember;
		*TestMember2 = *(OtherFoo.TestMember2);
	}

	Foo::~Foo()
	{
		delete TestMember2;
	}

	Foo& Foo::operator=(const Foo & Rhs)
	{
		if (this != &Rhs)
		{
			TestMember = Rhs.TestMember;
			*TestMember2 = *(Rhs.TestMember2);

		}
		return *this;
	}

	Foo& Foo::operator=(Foo &&Rhs)
	{
		delete TestMember2;
		TestMember = Rhs.TestMember;
		TestMember2 = Rhs.TestMember2;

		Rhs.TestMember = 0;
		Rhs.TestMember2 = nullptr;

		return *this;
	}

	std::wstring Foo::GetData() const
	{
		return std::wstring(std::to_wstring(TestMember) + std::to_wstring(*TestMember2));
	}

	bool Foo::operator==(const Foo& Rhs) const
	{

		if ((TestMember == Rhs.TestMember) && (*TestMember2 == *(Rhs.TestMember2)))
		{
			return true;
		}
		return false;
	}

	bool Foo::operator!=(const Foo& Rhs) const
	{
		if ((TestMember != Rhs.TestMember) || (*TestMember2 != *(Rhs.TestMember2)))
		{
			return true;
		}
		return false;
	}

	uint32_t Foo::operator()(const Foo & f1) const
	{
		return (((*(f1.TestMember2)) + f1.TestMember) * 7);
	}

	bool Foo::operator()(const Foo& f1, const Foo& f2) const
	{
		return(f1.TestMember == f2.TestMember);
	}

	uint32_t IntVectorIncrementStrategy::operator()(uint32_t size, uint32_t capacity) const
	{
		size;
		return (capacity * 2);
	}

	Foo::FooSharedData::FooSharedData(Foo& sharedFoo) : SharedFoo(&sharedFoo)
	{

	}

	FieaGameEngine::JsonParseMaster::SharedData* Foo::FooSharedData::Clone()
	{
		Foo *a = new Foo();
		FieaGameEngine::JsonParseMaster::SharedData *clone= new FooSharedData(*a);

		return clone;
	}
	Foo * Foo::FooSharedData::GetData()
	{
		return SharedFoo;
	}
	void FooParser::Initialize()
	{
	}
	bool FooParser::StartElementHandler(FieaGameEngine::JsonParseMaster::SharedData& sharedData, std::string & name, Json::Value & values)
	{
		name;
		sharedData;
		values;
		return false;
	}
	bool FooParser::ValueHandler(FieaGameEngine::JsonParseMaster::SharedData& sharedData, std::string& name, Json::Value& value)
	{
		bool result = false;
		Foo::FooSharedData *temp = (&sharedData)->As<Foo::FooSharedData>();
		if (temp != nullptr)
		{
			if (name == "TestMember")
			{
				temp->SharedFoo->TestMember = value.asInt();
			}
			else
			{
				*temp->SharedFoo->TestMember2 = value.asInt();
			}

			result = true;
			
		}

		return result;
	}

	bool FooParser::EndElementHandler(FieaGameEngine::JsonParseMaster::SharedData& sharedData)
	{
		sharedData;
		return false;
	}

	FieaGameEngine::JsonParseHelper* FooParser::Clone()
	{
		JsonParseHelper *clone = new FooParser();

		return clone;
	}
}