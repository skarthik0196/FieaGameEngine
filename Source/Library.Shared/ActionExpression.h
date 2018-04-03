#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	class ActionExpression : public Action
	{
		RTTI_DECLARATIONS(ActionExpression, Action)
	public:
		ActionExpression();
		ActionExpression(const std::string& name);
		ActionExpression(const ActionExpression& rhs);
		ActionExpression(ActionExpression&& rhs);
		~ActionExpression();

		ActionExpression& operator=(const ActionExpression& rhs);
		ActionExpression& operator=(ActionExpression&& rhs);

		void Update(WorldState& worldState);
		const std::string& GetRPN() const;
		void SetRPN(const std::string& rpn);

	private:

		enum class Operations
		{
			Addition,
			Subtraction,
			Multiplication,
			Division
		};

		void EvaluateRPN();

		bool IsOperator(std::string& token);
		void AddOperation(std::string& operand1, std::string& operand2);
		void SubtractOperation(std::string& operand1, std::string& operand2);
		void MultiplyOperation(std::string& operand1, std::string& operand2);
		void DivideOperation(std::string& operand1, std::string& operand2);

		void InitializeSignatures();
		void UpdateExternalStorage();
		std::string RPN;

		static HashMap<std::string, Operations> StringOperationMap;
	};
}

