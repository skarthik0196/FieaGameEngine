#pragma once
#include "ActionList.h"

namespace FieaGameEngine
{
	/// <summary>
	/// ActionListIf that derives from ActionList and emulates an If expression
	/// </summary>
	class ActionListIf : public ActionList
	{
		RTTI_DECLARATIONS(ActionListIf, ActionList)

	public:
		/// <summary>
		/// Default Constructor
		/// </summary>
		ActionListIf();

		/// <summary>
		/// Default Constructor that takes in a string to set Instance name of the action
		/// </summary>
		/// <param name="name"></param>
		explicit ActionListIf(const std::string& name);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs"></param>
		ActionListIf(const ActionListIf& rhs);

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs"></param>
		ActionListIf(ActionListIf&& rhs);

		/// <summary>
		/// Destructor
		/// </summary>
		~ActionListIf();

		/// <summary>
		/// Copy Assignment operator
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		ActionList& operator=(const ActionListIf& rhs);

		/// <summary>
		/// Move Assignment Operator
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		ActionList& operator=(ActionListIf&& rhs);

		/// <summary>
		/// Set the ConditionValue
		/// </summary>
		/// <param name="newValue"></param>
		void SetConditionValue(const int32_t& newValue);

		/// <summary>
		/// Set the IfBlock Action
		/// </summary>
		/// <param name="ifAction"></param>
		void SetIfBlock (Action& ifAction);

		/// <summary>
		/// Set the ElseBlock Action
		/// </summary>
		/// <param name="elseAction"></param>
		void SetElseBlock(Action& elseAction);

		/// <summary>
		/// Evaluate the condition and execute the appropriate actions
		/// </summary>
		/// <param name="worldState"></param>
		void Update(WorldState& worldState);

		void SetEvaluationBlock(Action& evaluationAction);

		static Vector<Signature> GetSignature();
			
	private:
		void InitializeSignatures();
		void UpdateExternalStorage();
		int32_t ConditionValue;
	};
}

