#include "pch.h"
#include "ActionExpression.h"
#include <sstream>
#include <stack>
#include <type_traits>

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionExpression)
	ActionExpression::ActionExpression():Action(TypeIdInstance())
	{
		//ActionExpression::InitializeSignatures();
	}

	ActionExpression::ActionExpression(const std::string& name) : Action(TypeIdInstance(), name)
	{
		//ActionExpression::InitializeSignatures();
	}

	ActionExpression::ActionExpression(const ActionExpression& rhs) : Action(rhs), RPN(rhs.RPN)
	{
		//ActionExpression::UpdateExternalStorage();
	}

	ActionExpression::ActionExpression(ActionExpression&& rhs) :Action(std::move(rhs)), RPN(std::move(rhs.RPN))
	{
		//ActionExpression::UpdateExternalStorage();
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
			//UpdateExternalStorage();
		}
		return *this;
	}

	ActionExpression& ActionExpression::operator=(ActionExpression&& rhs)
	{
		if (this != &rhs)
		{
			Action::operator=(std::move(rhs));
			RPN = std::move(rhs.RPN);
			//UpdateExternalStorage();
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

	Vector<Signature> ActionExpression::GetSignature()
	{
		Vector<Signature> signature = Action::GetSignature();
		signature.PushBack(Signature("RPN", Datum::DatumType::String, offsetof(ActionExpression, RPN), 1));

		return signature;
	}

	void ActionExpression::EvaluateRPN()
	{
		static void (ActionExpression::*OperateFunctions[5])(std::string&, std::string&) = {
																							&ActionExpression::AddOperation, 
																							&ActionExpression::SubtractOperation, 
																							&ActionExpression::MultiplyOperation, 
																							&ActionExpression::DivideOperation,
																							&ActionExpression::AssignmentOperation
																							};

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

				if (token != "=")
				{
					GetRealOperand(token1);
				}
				GetRealOperand(token2);

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
		std::vector<std::string> OperatorList = { "+", "-" , "*", "/", "=" };

		return (std::find(std::begin(OperatorList), std::end(OperatorList), token) != std::end(OperatorList));
	}

	void ActionExpression::AddOperation(std::string& operand1, std::string& operand2)
	{
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

	void ActionExpression::AssignmentOperation(std::string& operand1, std::string& operand2)
	{
		Datum* datum = Search(operand1);
		if (datum != nullptr)
		{
			datum->SetFromString(operand2, 0);
			operand1 = operand2;
		}
		else
		{
			throw std::runtime_error("Invalid Key");
		}
	}

	void ActionExpression::InitializeSignatures()
	{
		AddExternalAttribute("RPN", &RPN, 1);
	}

	void ActionExpression::UpdateExternalStorage()
	{
		UpdateStorage("RPN", &RPN, 1);
	}

	bool ActionExpression::IsLiteralNumber(const std::string & operand)
	{
		return (operand.find_first_not_of("0123456789.") == std::string::npos);
	}

	void ActionExpression::GetRealOperand(std::string& operand)
	{
		static std::string(*GetFunction[5])(Datum*) = { nullptr, 
														[](Datum* datum) {return std::to_string(datum->Get<int32_t>()); },
														[](Datum* datum) {return std::to_string(datum->Get<float>()); },
														[](Datum* datum) {return glm::to_string(datum->Get<glm::vec4>());},
														[](Datum* datum) {return glm::to_string(datum->Get<glm::mat4x4>()); }
													  };

		if (!IsLiteralNumber(operand))
		{
			Datum* datum = Search(operand);
			if (datum != nullptr)
			{
				operand = GetFunction[static_cast<uint32_t>(datum->GetType())](datum);
			}
			else
			{
				throw std::runtime_error("Invalid Key");
			}
		}
	}

	HashMap<std::string, ActionExpression::Operations> ActionExpression::StringOperationMap = {
																								{"+", Operations::Addition},
																								{"-", Operations::Subtraction},
																								{"*", Operations::Multiplication},
																								{"/", Operations::Division},
																								{"=", Operations::Assignment}
																							  };
}
