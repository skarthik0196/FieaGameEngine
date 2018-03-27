#include "pch.h"
#include "JsonParseHelperTable.h"
#include "AbstractFactory.h"
#include "ConcreteFactory.h"
#include "Sector.h"
#include "Entity.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonParseHelperTable::TableSharedData)

	JsonParseHelperTable::TableSharedData::TableSharedData(Scope& sharedScope) : SharedScope(&sharedScope)
	{
	}

	void JsonParseHelperTable::TableSharedData::Initialize()
	{

	}

	JsonParseMaster::SharedData* JsonParseHelperTable::TableSharedData::Clone()
	{
		Scope *clonedScope = new Scope();
		TableSharedData *clone = new TableSharedData(*clonedScope);

		return clone;
	}

	void JsonParseHelperTable::TableSharedData::SetSharedScope(Scope& sharedScope)
	{
		SharedScope = &sharedScope;
	}

	Scope* JsonParseHelperTable::TableSharedData::GetSharedScope()
	{
		return SharedScope;
	}

	void JsonParseHelperTable::Initialize()
	{
		while (ElementStack.size() > 0)
		{
			ElementStack.pop();
		}
	}

	bool JsonParseHelperTable::StartElementHandler(JsonParseMaster::SharedData& sharedData, std::string& name, Json::Value& values)
	{
		bool Result = false;
		TableSharedData* CustomSharedData = (&sharedData)->As<TableSharedData>();
		if (CustomSharedData != nullptr)
		{
			Result = true;
			
			if (values.get(values.getMemberNames()[0], values).type() == Json::ValueType::objectValue)
			{
				auto splitPosition = name.find("__");
				if (splitPosition != std::string::npos)
				{
					std::string instanceName = name.substr(0, splitPosition);
					std::string className = name.substr(splitPosition + 2, std::string::npos);
					CustomSharedData->SharedScope = static_cast<Scope*>(CustomSharedData->SharedScope->As<Sector>()->CreateEntity(className, instanceName));
				}
				else
				{
					CustomSharedData->SharedScope = &CustomSharedData->SharedScope->AppendScope(name);
				}
			}
			else
			{
				ElementStack.push(Element());
				ElementStack.top().ElementSignature["Name"] = name;
			}
		}

		values;
		return Result;
	}

	bool JsonParseHelperTable::ValueHandler(JsonParseMaster::SharedData& sharedData, std::string & name, Json::Value & value)
	{
		bool Result = false;
		TableSharedData* CustomSharedData = (&sharedData)->As<TableSharedData>();

		if (CustomSharedData != nullptr)
		{
			Result = true;
			ElementStack.top().ElementSignature[name] = value;
		}

		return Result;
	}

	bool JsonParseHelperTable::EndElementHandler(JsonParseMaster::SharedData& sharedData)
	{
		static void(JsonParseHelperTable::*AddDataPointer[9])(Json::Value&, Datum&) = { nullptr, &JsonParseHelperTable::AddIntData, &JsonParseHelperTable::AddFloatData, &JsonParseHelperTable::AddVectorOrMatrixData, &JsonParseHelperTable::AddVectorOrMatrixData, &JsonParseHelperTable::AddStringData, nullptr, nullptr, nullptr };

		bool Result = false;
		TableSharedData* CustomSharedData = (&sharedData)->As<TableSharedData>();

		if (CustomSharedData != nullptr)
		{
			Result = true;
			if (ElementStack.size() > 0)
			{
				Element currentElement = ElementStack.top();
				std::string currentElementName = currentElement.ElementSignature["Name"].asString();

				Datum& datum = CustomSharedData->SharedScope->Append(currentElementName);
				datum.SetType(StringTypeMap[currentElement.ElementSignature["type"].asString()]);
				datum.Resize(ElementStack.top().ElementSignature["size"].asUInt());

				Json::Value& currentValue = currentElement.ElementSignature["value"];

				(this->*AddDataPointer[static_cast<int32_t>(datum.GetType())])(currentValue, datum);

				ElementStack.pop();
			}
			else
			{
				if (CustomSharedData->SharedScope->GetParent() != nullptr)
				{
					CustomSharedData->SharedScope = CustomSharedData->SharedScope->GetParent();
				}
			}

		}

		return Result;
	}

	void JsonParseHelperTable::AddIntData(Json::Value& currentValue, Datum& datum)
	{
		if (currentValue.isArray() == true)
		{
			int index = 0;
			for (auto & value : currentValue)
			{
				datum.Set(value.asInt(), index);
				++index;
			}
		}
		else
		{
			datum.Set(currentValue.asInt(), 0);
		}
	}

	void JsonParseHelperTable::AddFloatData(Json::Value& currentValue, Datum& datum)
	{
		if (currentValue.isArray() == true)
		{
			int index = 0;
			for (auto & value : currentValue)
			{
				datum.Set(value.asFloat(), index);
				++index;
			}
		}
		else
		{
			datum.Set(currentValue.asFloat(), 0);
		}
	}

	void JsonParseHelperTable::AddStringData(Json::Value& currentValue, Datum& datum)
	{
		if (currentValue.isArray() == true)
		{
			int index = 0;
			for (auto & value : currentValue)
			{
				datum.Set(value.asString(), index);
				++index;
			}
		}
		else
		{
			datum.Set(currentValue.asString(), 0);
		}
	}

	void JsonParseHelperTable::AddVectorOrMatrixData(Json::Value& currentValue, Datum& datum)
	{
		datum.Resize(ElementStack.top().ElementSignature["size"].asUInt());
		if (currentValue.isArray())
		{
			int index = 0;
			for (auto& value : currentValue)
			{
				datum.SetFromString(value.asString(), index);
				++index;
			}
		}
		else
		{
			datum.SetFromString(currentValue.asString(), 0);
		}
	}

	JsonParseHelper * JsonParseHelperTable::Clone()
	{
		JsonParseHelperTable *clone = new JsonParseHelperTable();
		return clone;
	}

	HashMap<std::string, Datum::DatumType> JsonParseHelperTable::StringTypeMap = { {"Integer", Datum::DatumType::Integer}, {"Float", Datum::DatumType::Float}, {"String", Datum::DatumType::String}, {"Vector4", Datum::DatumType::Vector4}, {"Matrix4x4", Datum::DatumType::Matrix4x4} };

}
