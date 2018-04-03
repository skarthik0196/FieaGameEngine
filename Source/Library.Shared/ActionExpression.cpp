#include "pch.h"
#include "ActionExpression.h"
#include <sstream>
#include <stack>

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionExpression)
	ActionExpression::ActionExpression():Action()
	{
		ActionExpression::InitializeSignatures();
	}

	ActionExpression::ActionExpression(const std::string& name) : Action(name)
	{
		ActionExpression::InitializeSignatures();
	}

	ActionExpression::ActionExpression(const ActionExpression& rhs) : Action(rhs), RPN(rhs.RPN)
	{
		ActionExpression::UpdateExternalStorage();
	}

	ActionExpression::ActionExpression(ActionExpression&& rhs) :Action(std::move(rhs)), RPN(std::move(rhs.RPN))
	{
		ActionExpression::UpdateExternalStorage();
	}

	ActionExpression::~ActionExpression()
	{

	}

	ActionExpression & ActionExpression::operator=(const ActionExpression& rhs)
	{
		if (this != &rhs)
		{
			Action::operator=(rhs);
			RPN = rhs.RPN;
			UpdateExternalStorage();
		}
		return *this;
	}

	ActionExpression & ActionExpression::operator=(ActionExpression&& rhs)
	{
		if (this != &rhs)
		{
			Action::operator=(std::move(rhs));
			RPN = std::move(rhs.RPN);
			UpdateExternalStorage();
		}
		return *this;
	}

	void ActionExpression::Update(WorldState& worldState)
	{
		worldState;
		EvaluateRPN();
	}

	const std::string& ActionExpression::GetRPN() const
	{
		return RPN;
	}

	void ActionExpression::SetRPN(const std::string& rpn)
	{
		RPN = rpn;
	}

	void ActionExpression::EvaluateRPN()
	{
		static void (ActionExpression::*OperateFunctions[4])(std::string&, std::string&) = {&ActionExpression::AddOperation, &ActionExpression::SubtractOperation, &ActionExpression::MultiplyOperation, &ActionExpression::DivideOperation};

		std::stringstream RPNStream(RPN);
		std::string token;

		std::stack<std::string> OperandStack;
		
		while (RPNStream >> token)
		{
			if (IsOperator(token))
			{
				std::string token2 = OperandStack.top();
				OperandStack.pop();
				std::string token1 = OperandStack.top();
				OperandStack.pop();
				(this->*OperateFunctions[static_cast<uint32_t>(StringOperationMap[token])])(token1, token2);
				OperandStack.push(token1);
			}
			else
			{
				OperandStack.push(token);
			}
		}

		float result = std::stof(OperandStack.top());

		(*this)["Result"] = result;

	}

	bool ActionExpression::IsOperator(std::string & token)
	{
		std::vector<std::string> OperatorList = { "+", "-" , "*", "/" };

		return (std::find(std::begin(OperatorList), std::end(OperatorList), token) != std::end(OperatorList));
	}

	void ActionExpression::AddOperation(std::string& operand1, std::string& operand2)
	{
		/*float foperand1 = std::stof(operand1);
		float foperand2 = std::stof(operand2);*/
		operand1 = std::to_string((std::stof(operand1) + std::stof(operand2)));
	}

	void ActionExpression::SubtractOperation(std::string& operand1, std::string& operand2)
	{
		operand1 = std::to_string((std::stof(operand1) - std::stof(operand2)));
	}

	void ActionExpression::MultiplyOperation(std::string& operand1, std::string& operand2)
	{
		operand1 = std::to_string((std::stof(operand1) * std::stof(operand2)));
	}

	void ActionExpression::DivideOperation(std::string& operand1, std::string& operand2)
	{
		operand1 = std::to_string((std::stof(operand1) / std::stof(operand2)));
	}

	void ActionExpression::InitializeSignatures()
	{
		AddExternalAttribute("RPN", &RPN, 1);
	}

	void ActionExpression::UpdateExternalStorage()
	{
		UpdateStorage("RPN", &RPN, 1);
	}

	HashMap<std::string, ActionExpression::Operations> ActionExpression::StringOperationMap = {
																								{"+", Operations::Addition},
																								{"-", Operations::Subtraction},
																								{"*", Operations::Multiplication},
																								{"/", Operations::Division}
																							  };
}
