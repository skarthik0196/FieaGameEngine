#pragma once
#include "RTTI.h"
#include <json\json.h>
#include "Vector.h"

// Guard against depth

namespace FieaGameEngine
{
	class JsonParseHelper;

	/// <summary>
	/// The JsonParse Master class that is responsible for recieving parse requests from the client and handling the overarching parse process. This class implements the chain of responsibility pattern to handle the parsed data.
	/// </summary>
	class JsonParseMaster final
	{
	public:
		/// <summary>
		/// The base Shared Data class that will contain data after being parsed
		/// </summary>
		class SharedData : public RTTI
		{
			RTTI_DECLARATIONS(JsonParseMaster::SharedData, RTTI)

		public:
			/// <summary>
			/// Default constructor for Shared Data
			/// </summary>
			SharedData();

			/// <summary>
			/// Defaulted copy constructor
			/// </summary>
			/// <param name="rhs">Const refrence to SharedData to be copied from</param>
			SharedData(const SharedData& rhs) = default;

			/// <summary>
			/// Virtual defaulted destructor
			/// </summary>
			virtual ~SharedData() = default;

			/// <summary>
			/// The Initialize method that initializes Shared Data.
			/// </summary>
			virtual void Initialize();

			/// <summary>
			/// Clones the existing SharedData and returns a pointer to it, the owner of this data is the context from which it was called from so it must be handled appropriately
			/// </summary>
			/// <returns>A SharedData pointer pointing to the newly created Clone</returns>
			virtual SharedData* Clone();

			/// <summary>
			/// Sets the JsonParseMaster
			/// </summary>
			/// <param name="newParseMaster">A const reference to the new parse master</param>
			void SetJsonParseMaster(const JsonParseMaster& newParseMaster);

			/// <summary>
			/// Get the JsonParseMaster of Shared Data
			/// </summary>
			/// <returns>A pointer pointing to the JsonParseMaster</returns>
			JsonParseMaster* GetJsonParseMaster();
			
		private:
			JsonParseMaster* ParseMaster;
		};

		/// <summary>
		/// Default Constructor for JsonParsemaster
		/// </summary>
		JsonParseMaster() = default;

		/// <summary>
		/// Explicit  parameterized constructor for constructing JsonParseMaster from a SharedData
		/// </summary>
		/// <param name="data">A reference to the existing SharedData</param>
		explicit JsonParseMaster(SharedData& data);

		/// <summary>
		/// Destructor, frees memory that was allocated if the JsonParseMaster is a clone
		/// </summary>
		~JsonParseMaster();

		/// <summary>
		/// Defaulted copy constructor
		/// </summary>
		/// <param name="rhs">const reference to JsonParseMaster</param>
		JsonParseMaster(const JsonParseMaster& rhs) = default;

		/// <summary>
		/// Defaulted Move constructor
		/// </summary>
		/// <param name="rhs">const reference to JsonParseMaster</param>
		JsonParseMaster(JsonParseMaster&& rhs) = default;

		/// <summary>
		/// Defaulted copy assignment operator
		/// </summary>
		/// <param name="rhs">Const reference to rhs</param>
		/// <returns>A reference to JsonParseMaster</returns>
		JsonParseMaster& operator=(const JsonParseMaster& rhs) = default;

		/// <summary>
		/// Defaulted move assignment operator
		/// </summary>
		/// <param name="rhs">rvalue Reference to the JsonParseMaster to be moved</param>
		/// <returns>A reference to JsonParseMaster</returns>
		JsonParseMaster& operator=(JsonParseMaster&& rhs) = default;

		/// <summary>
		/// Adds a helper to the JsonParseMaster's list of helpers, Adding is not allowed for clones
		/// </summary>
		/// <param name="newHandler">A reference to the helper to be added</param>
		void AddHelper(JsonParseHelper& newHandler);
		
		/// <summary>
		/// Removes a helper from JsonParseMaster's list of helpers
		/// </summary>
		/// <param name="handlerToRemove">A reference to the helper to be removed</param>
		void RemoveHelper(JsonParseHelper& handlerToRemove);

		/// <summary>
		/// Initializes the JsonParseMaster
		/// </summary>
		void Initialize();

		/// <summary>
		/// Resets the JsonParseMaster - functionally equivalent to Initialize
		/// </summary>
		void Reset();

		/// <summary>
		/// Parse the Json data that was sent in as a string
		/// </summary>
		/// <param name="buffer">A const std::string reference to the Json data</param>
		/// <returns>A bool reprsenting if the data was parsed successfully</returns>
		bool Parse(const std::string& buffer);

		/// <summary>
		/// Parse the Json data that was sent in as a InputStream
		/// </summary>
		/// <param name="inputStream">An Ifstream& to the Json data</param>
		/// <returns>A bool reprsenting if the data was parsed successfully</returns>
		bool Parse(std::ifstream& inputStream);

		/// <summary>
		/// Parse Json Data from a Json file
		/// </summary>
		/// <param name="Filename">The path of the Json file with extension</param>
		/// <returns>A bool reprsenting if the data was parsed successfully</returns>
		bool ParseFromFile(const std::string& Filename);
		
		/// <summary>
		/// Returns the SharedData object that is currently being used by the JsonParseMaster
		/// </summary>
		/// <returns>A pointer to the SharedData</returns>
		SharedData* GetSharedData() const;

		/// <summary>
		/// Set the SharedData object for further use by the JsonParseMaster
		/// </summary>
		/// <param name="newSharedData">A reference to the shared data to be set</param>
		void SetSharedData(SharedData& newSharedData);

		/// <summary>
		/// Clones the existing the JsonParseMaster and returns a pointer to it, The context that called this function is now responsible for the memory of the cloned object
		/// </summary>
		/// <returns></returns>
		JsonParseMaster* Clone();

		/// <summary>
		/// Returns whether the ParseMaster is a clone
		/// </summary>
		/// <returns>A bool determining whether the JsonParseMaster is a clone or not</returns>
		bool IsParseMasterClone() const;

		/// <summary>
		/// Returns the file path that is currently set in the JsonParseMaster
		/// </summary>
		/// <returns>A const reference to a string that holds the filepath information</returns>
		const std::string& GetFileName() const;

	private:
		void HandleParsedData(bool& result, Json::Value& JsonValue);

		SharedData * Data { nullptr };
		bool IsClone{ false };
		Vector<JsonParseHelper*> JsonParseHandlers;
		std::string FilePath;
	};
}

