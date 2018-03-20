#pragma once
#include"JsonParseMaster.h"

namespace FieaGameEngine
{
	/// <summary>
	/// The abstract JsonParseHelper class that all helpers derive from
	/// </summary>
	class JsonParseHelper
	{

	public:
		/// <summary>
		/// Defaulted Constructor
		/// </summary>
		JsonParseHelper() = default;

		/// <summary>
		/// Virtual default Destructor
		/// </summary>
		virtual ~JsonParseHelper() = default;

		/// <summary>
		/// Defaulted copy constructor
		/// </summary>
		/// <param name="rhs">A const reference to JsonParseHelper</param>
		JsonParseHelper(const JsonParseHelper& rhs) = default;

		/// <summary>
		/// Defaulted Move constructor
		/// </summary>
		/// <param name="rhs">A reference to JsonParseHelper</param>
		JsonParseHelper(JsonParseHelper&& rhs) = default;

		/// <summary>
		/// Defaulted copy assignment operator
		/// </summary>
		/// <param name="rhs">A const reference to JsonParseHelper</param>
		/// <returns>A reference to JsonParseHelper</returns>
		JsonParseHelper& operator=(const JsonParseHelper& rhs) = default;

		/// <summary>
		/// Defaulted Move Assignment operator
		/// </summary>
		/// <param name="rhs">A rvalue reference to JsonParseHelper</param>
		/// <returns>A reference to JsonParseHelper</returns>
		JsonParseHelper& operator=(JsonParseHelper&& rhs) = default;

		/// <summary>
		/// Virtual Initialize method
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Pure virtual Start Element Handler function
		/// </summary>
		/// <param name="sharedData">A reference to the SharedData to be used while parsing</param>
		/// <param name="name">The current key</param>
		/// <param name="values">The JsonValue associated with that key</param>
		/// <returns>A bool indicating whether the handler can handle the passed SharedData</returns>
		virtual bool StartElementHandler(JsonParseMaster::SharedData& sharedData, std::string& name, Json::Value& values) = 0;

		/// <summary>
		/// Pure virtual Value Element Handler function
		/// </summary>
		/// <param name="sharedData">A reference to the SharedData to be used while parsing</param>
		/// <param name="name">The current key</param>
		/// <param name="values">The JsonValue associated with that key</param>
		/// <returns>A bool indicating whether the handler can handle the passed SharedData</returns>
		virtual bool ValueHandler(JsonParseMaster::SharedData& sharedData, std::string& name, Json::Value& values) = 0;

		/// <summary>
		/// Pure virtual End Element Handler function
		/// </summary>
		/// <param name="sharedData">A reference to the SharedData to be used while parsing</param>
		/// <returns>A bool indicating whether the handler can handle the passed SharedData</returns>
		virtual bool EndElementHandler(JsonParseMaster::SharedData& sharedData) = 0;

		/// <summary>
		/// Virtual Clone function that returns clones the helper and returns a pointer to cloned helper
		/// </summary>
		/// <returns></returns>
		virtual JsonParseHelper* Clone() = 0;
	};
}

