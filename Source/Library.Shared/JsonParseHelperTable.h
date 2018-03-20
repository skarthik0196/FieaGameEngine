#pragma once
#include "JsonParseHelper.h"
#include "JsonParseMaster.h"
#include "Scope.h"

namespace FieaGameEngine
{
	/// <summary>
	/// The Table Parse helper that handles request for parsing Scopes sent in from the ParseMaster, This class derives from JsonParseHelper
	/// </summary>
	class JsonParseHelperTable : public JsonParseHelper
	{
	public:
		/// <summary>
		/// The TableSharedData class that holds a Scope Pointer, this derives from SharedData in JsonParseMaster
		/// </summary>
		class TableSharedData : public JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(TableSharedData, SharedData)

			friend class JsonParseHelperTable;

		public:
			/// <summary>
			/// Explicit constructor to Table Shared Data, It requires that SharedData be initialzed with an already existing scope
			/// </summary>
			/// <param name="sharedScope">A reference to the shared scope</param>
			explicit TableSharedData(Scope& sharedScope);

			/// <summary>
			/// Destructor of TableSharedData
			/// </summary>
			~TableSharedData() = default;

			/// <summary>
			/// Initialize function that initializes the Table SharedData
			/// </summary>
			virtual void Initialize();
			
			/// <summary>
			/// Clones the table helper, the context that calls clone in the master is now responsible for the memory of the new clone object
			/// </summary>
			/// <returns>A SharedData pointer pointing to the newly cloned TableHelper</returns>
			virtual SharedData* Clone();

			/// <summary>
			/// Get the currently held SharedScope
			/// </summary>
			/// <returns>A scope pointer to the currently held SharedScope</returns>
			Scope* GetSharedScope();

			/// <summary>
			/// Set the SharedScope to be used
			/// </summary>
			/// <param name="rhs">A reference to the SharedScope</param>
			void SetSharedScope(Scope& rhs);

		private:
			Scope *SharedScope{ nullptr };
		};

		/// <summary>
		/// Defaulted constructor
		/// </summary>
		JsonParseHelperTable() = default;

		/// <summary>
		/// Defaulted Destructor
		/// </summary>
		virtual ~JsonParseHelperTable() = default;

		/// <summary>
		/// Defaulted copy constructor
		/// </summary>
		/// <param name="rhs">A const reference to JsonParseHelperTable</param>
		JsonParseHelperTable(const JsonParseHelperTable& rhs) = default;

		/// <summary>
		/// Defaulted move constructor
		/// </summary>
		/// <param name="rhs">A const reference to JsonParseHelperTable</param>
		JsonParseHelperTable(JsonParseHelperTable&& rhs) = default;

		/// <summary>
		/// Initializes the Table helper
		/// </summary>
		void Initialize();

		/// <summary>
		/// Start Element Handler that is responsible for determining if the table can handle the data.
		/// </summary>
		/// <param name="sharedData">A reference to the SharedData to be used while parsing</param>
		/// <param name="name">The current key</param>
		/// <param name="values">The JsonValue associated with that key</param>
		/// <returns>A bool indicating whether the tablehandler can handle the passed SharedData</returns>
		bool StartElementHandler(JsonParseMaster::SharedData& sharedData, std::string& name, Json::Value& values);

		/// <summary>
		/// Value Element Handler that handles the intermediate elements that come in from the ParseMaster
		/// </summary>
		/// <param name="sharedData">A reference to the SharedData to be used while parsing</param>
		/// <param name="name">The current key</param>
		/// <param name="values">The JsonValue associated with that key</param>
		/// <returns>A bool indicating whether the tablehandler can handle the passed SharedData</returns>
		bool ValueHandler(JsonParseMaster::SharedData& sharedData, std::string& name, Json::Value& values);

		/// <summary>
		/// End Element Handler
		/// </summary>
		/// <param name="sharedData">A reference to the SharedData to be used while parsing</param>
		/// <returns>A bool indicating whether the table handler can handle the passed SharedData</returns>
		bool EndElementHandler(JsonParseMaster::SharedData& sharedData);

		/// <summary>
		/// Clones the table helper, the context that cloned the helper is now responsible for it's memory and deletion
		/// </summary>
		/// <returns>A JsonParseHelper pointer</returns>
		virtual JsonParseHelper* Clone();

	private:
		void AddIntData(Json::Value& value, Datum& datum);
		void AddFloatData(Json::Value& value, Datum& datum);
		void AddStringData(Json::Value& value, Datum& datum);
		void AddVectorOrMatrixData(Json::Value& value, Datum& datum);

		class Element
		{
			friend class JsonParseHelperTable;

		private:
			HashMap<std::string, Json::Value> ElementSignature;
		};

		std::stack<Element> ElementStack;

		static HashMap<std::string, Datum::DatumType> StringTypeMap;
		
		
	};
}

