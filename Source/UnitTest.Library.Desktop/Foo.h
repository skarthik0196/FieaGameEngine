#pragma once
#include "Vector.h"
#include "RTTI.h"
#include "JsonParseHelper.h"
#include "JsonParseMaster.h"

namespace UnitTestLibraryDesktop
{
	class Foo : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Foo, FieaGameEngine::RTTI)
	friend class FooParser;
	private:
		int TestMember;
		int *TestMember2;

	public:
		class FooSharedData : public FieaGameEngine::JsonParseMaster::SharedData
		{
			friend class FooParser;
			RTTI_DECLARATIONS(FooSharedData, FieaGameEngine::JsonParseMaster::SharedData)
		public:
			explicit FooSharedData(Foo& SharedFoo);
			SharedData* Clone();
			Foo* GetData();

		private:
			Foo * SharedFoo;
		};

		Foo(int Value = 0, int Value2 = 0);
		Foo(const Foo& OtherFoo);
		~Foo();
		Foo& operator=(const Foo &Rhs);
		Foo& operator=(Foo &&Rhs);

		std::wstring GetData() const;

		bool operator==(const Foo&Rhs) const;
		bool operator!=(const Foo&Rhs) const;

		uint32_t operator()(const Foo& f1) const;
		bool operator()(const Foo& f1, const Foo& f2) const;
	};

	class IntVectorIncrementStrategy
	{
	public:
		uint32_t operator()(uint32_t size, uint32_t capacity) const;
	};

	class FooParser : public  FieaGameEngine::JsonParseHelper
	{
	public:
		void Initialize();

		bool StartElementHandler(FieaGameEngine::JsonParseMaster::SharedData& sharedData, std::string& name, Json::Value& value);
		bool ValueHandler(FieaGameEngine::JsonParseMaster::SharedData& sharedData, std::string& name, Json::Value& values);
		bool EndElementHandler(FieaGameEngine::JsonParseMaster::SharedData& sharedData);
		JsonParseHelper* Clone();
		
	};
}