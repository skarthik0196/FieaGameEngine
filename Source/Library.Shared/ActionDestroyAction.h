#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Action Destroy action that destroys an action
	/// </summary>
	class ActionDestroyAction : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction, Action)

	public:
		/// <summary>
		/// Default Constructor
		/// </summary>
		ActionDestroyAction();

		/// <summary>
		/// Default Constructor with string to set name
		/// </summary>
		/// <param name="name"></param>
		explicit ActionDestroyAction(const std::string& name);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs"></param>
		ActionDestroyAction(const ActionDestroyAction& rhs);

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs"></param>
		ActionDestroyAction(ActionDestroyAction&& rhs);
		~ActionDestroyAction();

		/// <summary>
		/// Copy Assignment operator
		/// </summary>
		/// <param name="rhs">Refernce to another ActionDestroyAction</param>
		/// <returns>Reference</returns>
		ActionDestroyAction& operator=(const ActionDestroyAction& rhs);

		/// <summary>
		/// Move Assignment operator
		/// </summary>
		/// <param name="rhs">R-value reference to another ActionDestroyAction</param>
		/// <returns>Reference</returns>
		ActionDestroyAction& operator=(ActionDestroyAction&& rhs);

		/// <summary>
		/// Update Method that adds Actions to the kill list
		/// </summary>
		/// <param name="worldState">World State</param>
		void Update(WorldState& worldState);

		/// <summary>
		/// Set Action Instance Name
		/// </summary>
		/// <param name="actionInstanceName">String containing name of the action instance</param>
		void SetActionInstanceName(const std::string& actionInstanceName);

		/// <summary>
		/// Get ActionInstanceName
		/// </summary>
		/// <returns>String</returns>
		const std::string& GetActionInstanceName();

		/// <summary>
		/// Mark self for kill
		/// </summary>
		void AddSelfToKillList();

	private:
		void InitializeSignatures();
		void UpdateExternalStorage();

		std::string ActionInstanceName;
	};
}

