#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Create Action class that derives from Action that performs the action of creating a class
	/// </summary>
	class ActionCreateAction : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action)
	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		ActionCreateAction();

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs"></param>
		ActionCreateAction(const ActionCreateAction& rhs);

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs"></param>
		ActionCreateAction(ActionCreateAction&& rhs);

		/// <summary>
		/// Destructor
		/// </summary>
		~ActionCreateAction();

		/// <summary>
		/// Copy Assignment operator
		/// </summary>
		/// <param name="rhs">Reference to ActionCreateAction</param>
		/// <returns>Reference to ActionCreateAction</returns>
		ActionCreateAction& operator=(const ActionCreateAction& rhs);

		/// <summary>
		/// Move Assignment operator
		/// </summary>
		/// <param name="rhs">R value reference to ActionCreateAction</param>
		/// <returns>Reference to ActionCreateAction</returns>
		ActionCreateAction& operator=(ActionCreateAction&& rhs);
		
		/// <summary>
		/// Set Class name of action to be created
		/// </summary>
		/// <param name="actionClassName">String containing name of the new class</param>
		void SetActionClassName(const std::string& actionClassName);

		/// <summary>
		/// Set Instance name of the action to be created
		/// </summary>
		/// <param name="actionInstanceName">string containing instance name</param>
		void SetActionInstanceName(const std::string& actionInstanceName);

		/// <summary>
		/// Get Action Class name
		/// </summary>
		/// <returns>String</returns>
		const std::string& GetActionClassName() const;

		/// <summary>
		/// Get Action Instance Name
		/// </summary>
		/// <returns>String</returns>
		const std::string& GetActionInstanceName() const;

		/// <summary>
		/// Update method which creates the action
		/// </summary>
		/// <param name="worldState">Worldstate reference</param>
		void Update(WorldState& worldState);

		static Vector<Signature> GetSignature();

	private:
		void InitializeSignatures();
		void UpdateExternalStorage();

		std::string ActionClassName;
		std::string ActionInstanceName;
	};
}

