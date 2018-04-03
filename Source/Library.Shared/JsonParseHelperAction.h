#pragma once
#include "JsonParseHelper.h"
#include "JsonParseMaster.h"
#include "Action.h"

namespace FieaGameEngine
{
	class JsonParseHelperAction : public JsonParseHelper
	{
	public:
		class EntitySharedData : public JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(EntitySharedData, SharedData)
		public:
			friend class JsonParseHelperAction;

			explicit EntitySharedData(Entity& entity);
			~EntitySharedData() = default;

			void Initialize();
			SharedData* Clone();
			void SetSharedEntity(Entity& entity);
			Entity* GetSharedEntity();

		private:
			Entity *SharedEntity{ nullptr };
			Action *CurrentAction{ nullptr };
		};
	
		JsonParseHelperAction() = default;
		JsonParseHelperAction(const JsonParseHelperAction& rhs) = default;
		JsonParseHelperAction(JsonParseHelperAction&& rhs) = default;
		~JsonParseHelperAction() = default;

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

		void InnerActionParse(std::string& name, Json::Value& values, JsonParseHelperAction::EntitySharedData& customSharedData);
		void InnerIfParse(std::string& name, Json::Value& values, JsonParseHelperAction::EntitySharedData& customSharedData);
		void ExpressionParse(std::string& name, Json::Value& values, JsonParseHelperAction::EntitySharedData& customSharedData);

		void ConvertToRPN(std::string& expression);
		bool IsOperator(const std::string& token);
		bool IsHigherOrSamePriority(std::string& newOperator, std::string& stackTop);

		void AddIntData(Json::Value& value, Datum& datum);
		void AddFloatData(Json::Value& value, Datum& datum);
		void AddStringData(Json::Value& value, Datum& datum);
		void AddVectorOrMatrixData(Json::Value& value, Datum& datum);

		enum class ActionTypes
		{
			None,
			ActionListIf,
			ActionExpression,
			TestAction,
			ActionList,
			NA,
		};

		class Element
		{
		public:
			HashMap<std::string, Json::Value> ElementSignature;
		};

		std::stack<Element> ElementStack;
		std::stack<ActionTypes> ActionTypeStack;

		ActionTypes ParsingAction{ActionTypes::None};
		static HashMap<std::string, Datum::DatumType> StringTypeMap;
		static HashMap<std::string, ActionTypes> StringActionMap;
		static HashMap<std::string, uint32_t> OperatorWeightMap;
	};
}

