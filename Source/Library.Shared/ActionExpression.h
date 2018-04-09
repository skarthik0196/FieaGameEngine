#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Action Expression class to evaluate an expression, Currently support '+' , '-' ,'*' ,'/' and '=' operators
	/// </summary>
	class ActionExpression : public Action
	{
		RTTI_DECLARATIONS(ActionExpression, Action)
	public:
		/// <summary>
		/// Default Constructor
		/// </summary>
		ActionExpression();

		/// <summary>
		/// Default Constructor that takes in a string to set instance name
		/// </summary>
		/// <param name="name"></param>
		explicit ActionExpression(const std::string& name);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs"></param>
		ActionExpression(const ActionExpression& rhs);

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs"></param>
		ActionExpression(ActionExpression&& rhs);
		~ActionExpression();

		/// <summary>
		/// Copy Assignement operator
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		ActionExpression& operator=(const ActionExpression& rhs);

		/// <summary>
		/// Move Assignment operator
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		ActionExpression& operator=(ActionExpression&& rhs);

		/// <summary>
		/// Update method that evaluates the expression and also stores the result in the Result datum
		/// </summary>
		/// <param name="worldState"></param>
		void Update(WorldState& worldState);

		/// <summary>
		/// Get the Current RPN that's being evaluated
		/// </summary>
		/// <returns>String</returns>
		const std::string& GetRPN() const;

		/// <summary>
		/// Set the RPN to be evaluated
		/// </summary>
		/// <param name="rpn">String containing new RPN</param>
		void SetRPN(const std::string& rpn);

		static Vector<Signature> GetSignature();

	private:

		enum class Operations
		{
			Addition,
			Subtraction,
			Multiplication,
			Division,
			Assignment
		};

		void EvaluateRPN();

		bool IsOperator(std::string& token);
		void AddOperation(std::string& operand1, std::string& operand2);
		void SubtractOperation(std::string& operand1, std::string& operand2);
		void MultiplyOperation(std::string& operand1, std::string& operand2);
		void DivideOperation(std::string& operand1, std::string& operand2);
		void AssignmentOperation(std::string& operand1, std::string& operand2);

		void InitializeSignatures();
		void UpdateExternalStorage();

		bool IsLiteralNumber(const std::string& operand);
		void GetRealOperand(std::string& operand);

		std::string RPN;

		static HashMap<std::string, Operations> StringOperationMap;
	};
}

