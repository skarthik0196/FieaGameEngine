#pragma once
#include "pch.h"
#include <fstream>
#include "JsonParseMaster.h"
#include "JsonParseHelper.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonParseMaster::SharedData)

	JsonParseMaster::SharedData::SharedData()
	{
		
	}

	void JsonParseMaster::SharedData::SetJsonParseMaster(const JsonParseMaster& newParseMaster)
	{
		ParseMaster = const_cast<JsonParseMaster*>(&newParseMaster);
	}

	JsonParseMaster* JsonParseMaster::SharedData::GetJsonParseMaster()
	{
		return ParseMaster;
	}

	JsonParseMaster::SharedData* JsonParseMaster::SharedData::Clone()
	{
		SharedData *clone = new SharedData(*this);
		
		return clone;
	}

	void JsonParseMaster::SharedData::Initialize()
	{

	}

	JsonParseMaster::JsonParseMaster(SharedData& data) : Data(&data)
	{
		data.SetJsonParseMaster(*this);
	}


	JsonParseMaster::~JsonParseMaster()
	{
		if (IsClone == true)
		{
			for (auto& value : JsonParseHandlers)
			{
				delete value;
			}
			delete Data;
		}
	}

	void JsonParseMaster::AddHelper(JsonParseHelper& newHandler)
	{
		if (IsClone == true)
		{
			throw std::runtime_error("Cannot Add Helper to Clone");
		}
		JsonParseHandlers.PushBack(&newHandler);
	}

	void JsonParseMaster::RemoveHelper(JsonParseHelper& handlerToRemove)
	{
		auto It = JsonParseHandlers.Find(&handlerToRemove);
		JsonParseHandlers.Remove(It, It);
	}

	
	JsonParseMaster* JsonParseMaster::Clone()
	{
		SharedData* newSharedData= Data->Clone();
		newSharedData->SetJsonParseMaster(*this);
		JsonParseMaster *clone = new JsonParseMaster(*newSharedData);

		for (auto& value : JsonParseHandlers)
		{
			clone->AddHelper(*(value->Clone()));
		}

		clone->IsClone = true;
		clone->FilePath.clear();

		return clone;
	}

	bool JsonParseMaster::Parse(std::ifstream& inputStream)
	{
		Initialize();

		Json::Value jsonRoot;
		bool result = false;

		inputStream >> jsonRoot;
		HandleParsedData(result, jsonRoot);

		return result;
	}

	bool JsonParseMaster::Parse(const std::string& buffer)
	{
		Initialize();

		Json::Value jsonRoot;

		bool result = false;
		std::stringstream StringStream;
		StringStream.str(buffer);
		StringStream >> jsonRoot;

		HandleParsedData(result, jsonRoot);

		return result;
	}


	bool JsonParseMaster::ParseFromFile(const std::string& filename)
	{
		Initialize();

		Json::Value jsonRoot;
		bool result;

		FilePath = filename;
		std::ifstream fileStream(FilePath.c_str(), std::ios::binary);

		if (fileStream.fail())
		{
			throw std::runtime_error("File Does not Exist");
		}

		fileStream >> jsonRoot;

		HandleParsedData(result, jsonRoot);

		return result;
	}

	void JsonParseMaster::HandleParsedData(bool& result, Json::Value& JsonValue)
	{
		std::vector<std::string> KeyList = JsonValue.getMemberNames();

		for (auto &currentKey : KeyList)
		{
			Json::Value& currentJsonValue = JsonValue[currentKey];
			for (auto& value : JsonParseHandlers)
			{
				if (currentJsonValue.type() == Json::ValueType::objectValue)
				{
					if (value->StartElementHandler(*Data, currentKey, currentJsonValue) == true)
					{
						result = true;
						HandleParsedData(result, currentJsonValue);
						break;
					}
				}
				else
				{
					if (value->ValueHandler(*Data, currentKey, currentJsonValue) == true)
					{
						result = true;
						break;
					}
				}
			}
		}

		if (result == true)
		{
			for (auto& value : JsonParseHandlers)
			{
				if (value->EndElementHandler(*Data) == true)
				{
					break;
				}
			}
		}

	}

	const std::string& JsonParseMaster::GetFileName() const
	{
		return FilePath;
	}

	bool JsonParseMaster::IsParseMasterClone() const
	{
		return IsClone;
	}

	JsonParseMaster::SharedData* JsonParseMaster::GetSharedData() const
	{
		return Data;
	}

	void JsonParseMaster::SetSharedData(SharedData& newSharedData)
	{
		Data = &newSharedData;
		Data->SetJsonParseMaster(*this);
	}

	void JsonParseMaster::Initialize()
	{
		Data->Initialize();
		for (auto& value : JsonParseHandlers)
		{
			value->Initialize();
		}
		FilePath.clear();
	}

	void JsonParseMaster::Reset()
	{
		Initialize();
	}
}