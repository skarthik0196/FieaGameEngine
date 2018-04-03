#include "pch.h"
#include "JsonParseHelperAction.h"
#include "Entity.h"
#include "AbstractFactory.h"
#include "ConcreteFactory.h"
#include "ActionListIf.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonParseHelperAction::EntitySharedData)

	JsonParseHelperAction::EntitySharedData::EntitySharedData(Entity& entity) : SharedEntity(&entity)
	{

	}

	void JsonParseHelperAction::EntitySharedData::Initialize()
	{
	
	}

	JsonParseMaster::SharedData* JsonParseHelperAction::EntitySharedData::Clone()
	{
		Entity *clonedEntity = new Entity();
		EntitySharedData *clonedData = new EntitySharedData(*clonedEntity);

		return clonedData;
	}

	void JsonParseHelperAction::EntitySharedData::SetSharedEntity(Entity& entity)
	{
		SharedEntity = &entity;
	}

	Entity * JsonParseHelperAction::EntitySharedData::GetSharedEntity()
	{
		return SharedEntity;
	}

	void JsonParseHelperAction::Initialize()
	{
	}

	bool JsonParseHelperAction::StartElementHandler(JsonParseMaster::SharedData& sharedData, std::string& name, Json::Value& values)
	{

		static void(JsonParseHelperAction::*InnerDataPointer[3])(std::string&, Json::Value&, JsonParseHelperAction::EntitySharedData&) = {&JsonParseHelperAction::InnerActionParse, &JsonParseHelperAction::InnerIfParse, nullptr};

		bool result = false;
		EntitySharedData *CustomSharedData = sharedData.As <EntitySharedData>();
		if (CustomSharedData != nullptr)
		{
			result = true;
			if (values.get(values.getMemberNames()[0], values).type() == Json::ValueType::objectValue)
			{
				(this->*InnerDataPointer[static_cast<uint32_t>(ParsingAction)])(name, values, *CustomSharedData);
			}
			else
			{
				ElementStack.push(Element());
				ElementStack.top().ElementSignature["Name"] = name;
				//ParsingAction = ActionTypes::NA;
			}

		}
		return result;
	}

	bool JsonParseHelperAction::ValueHandler(JsonParseMaster::SharedData& sharedData, std::string& name, Json::Value& values)
	{
		bool result = false;
		EntitySharedData *CustomSharedData = sharedData.As <EntitySharedData>();
		if (CustomSharedData != nullptr)
		{
			result = true;
			ElementStack.top().ElementSignature[name] = values;
		}
		return result;
	}

	bool JsonParseHelperAction::EndElementHandler(JsonParseMaster::SharedData& sharedData)
	{
		static void(JsonParseHelperAction::*AddDataPointer[9])(Json::Value&, Datum&) = { nullptr, &JsonParseHelperAction::AddIntData, &JsonParseHelperAction::AddFloatData, &JsonParseHelperAction::AddVectorOrMatrixData, &JsonParseHelperAction::AddVectorOrMatrixData, &JsonParseHelperAction::AddStringData, nullptr, nullptr, nullptr };

		bool result = false;
		EntitySharedData *CustomSharedData = sharedData.As <EntitySharedData>();
		if (CustomSharedData != nullptr)
		{
			result = true;
			if (ElementStack.size() > 0)
			{
				Element& currentElement = ElementStack.top();
				Datum& datum = CustomSharedData->CurrentAction->Append(currentElement.ElementSignature["Name"].asString());
				datum.SetType(StringTypeMap[currentElement.ElementSignature["type"].asString()]);

				datum.Resize(ElementStack.top().ElementSignature["size"].asUInt());

				Json::Value& currentValue = currentElement.ElementSignature["value"];

				(this->*AddDataPointer[static_cast<int32_t>(datum.GetType())])(currentValue, datum);
				ElementStack.pop();
			}
			else
			{
				if (!CustomSharedData->CurrentAction->GetParent()->Is("Entity"))
				{
					CustomSharedData->CurrentAction = static_cast<Action*>(CustomSharedData->CurrentAction->GetParent());
					ActionTypeStack.pop();
					if (ActionTypeStack.size() > 0)
					{
						ParsingAction = ActionTypeStack.top();
					}
				}
				else
				{
					while (ActionTypeStack.size() > 0)
					{
						ActionTypeStack.pop();
					}
					ParsingAction = ActionTypes::None;
				}
			}
		}

		return result;
	}

	JsonParseHelper * JsonParseHelperAction::Clone()
	{
		return nullptr;
	}

	void JsonParseHelperAction::InnerActionParse(std::string& name, Json::Value& values, JsonParseHelperAction::EntitySharedData& customSharedData)
	{
		auto splitPosition = name.find("__");
		std::string instanceName = name.substr(0, splitPosition);
		std::string className = name.substr(splitPosition + 2, std::string::npos);

		ParsingAction = StringActionMap[className];
		ActionTypeStack.push(ParsingAction);

		customSharedData.CurrentAction = customSharedData.SharedEntity->CreateAction(className, instanceName);

		values;
	}

	void JsonParseHelperAction::InnerIfParse(std::string& name, Json::Value& values, JsonParseHelperAction::EntitySharedData& customSharedData)
	{
		auto splitPosition = name.find("__");
		std::string block = name.substr(0, splitPosition);
		std::string className = name.substr(splitPosition + 2, std::string::npos);
		values;
		if (block == "then")
		{
			Action* thenAction = AbstractFactory<Action>::Create(className);
			assert(customSharedData.CurrentAction->Is("ActionListIf"));
			static_cast<ActionListIf*>(customSharedData.CurrentAction)->SetIfBlock(*thenAction);

			customSharedData.CurrentAction = thenAction;
			ParsingAction = StringActionMap[className];
			ActionTypeStack.push(ParsingAction);
		}
		else
		{
			Action* elseAction = AbstractFactory<Action>::Create(className);
			assert(customSharedData.CurrentAction->Is("ActionListIf"));
			static_cast<ActionListIf*>(customSharedData.CurrentAction)->SetElseBlock(*elseAction);

			customSharedData.CurrentAction = elseAction;
			ParsingAction = StringActionMap[className];
			ActionTypeStack.push(ParsingAction);
		}

	}


	void JsonParseHelperAction::AddIntData(Json::Value& currentValue, Datum& datum)
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

	void JsonParseHelperAction::AddFloatData(Json::Value& currentValue, Datum& datum)
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

	void JsonParseHelperAction::AddStringData(Json::Value& currentValue, Datum& datum)
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

	void JsonParseHelperAction::AddVectorOrMatrixData(Json::Value& currentValue, Datum& datum)
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


	HashMap<std::string, JsonParseHelperAction::ActionTypes> JsonParseHelperAction::StringActionMap = { 
																									{"ActionListIf", JsonParseHelperAction::ActionTypes::ActionListIf}, 
																									{"ActionExpression", JsonParseHelperAction::ActionTypes::ActionExpression},
																									{"TestAction",  JsonParseHelperAction::ActionTypes::TestAction}
																									};

	HashMap<std::string, Datum::DatumType> JsonParseHelperAction::StringTypeMap = { { "Integer", Datum::DatumType::Integer },{ "Float", Datum::DatumType::Float },{ "String", Datum::DatumType::String },{ "Vector4", Datum::DatumType::Vector4 },{ "Matrix4x4", Datum::DatumType::Matrix4x4 } };
}
