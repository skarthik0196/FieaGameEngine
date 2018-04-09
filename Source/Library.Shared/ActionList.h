#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Action list that derives from Action and stores a list of Action
	/// </summary>
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action)

	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		ActionList();

		/// <summary>
		/// Default constructor that takes in a string to set instance name
		/// </summary>
		/// <param name="name"></param>
		explicit ActionList(const std::string& name);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs"></param>
		ActionList(const ActionList& rhs);

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs"></param>
		ActionList(ActionList&& rhs);

		virtual ~ActionList();

		/// <summary>
		/// Copy Assignment operator
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		ActionList& operator=(const ActionList& rhs);

		/// <summary>
		/// Move Assignment operator
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		ActionList& operator=(ActionList&& rhs);
		
		/// <summary>
		/// Get the datum containing the list of actions the ActionList holds
		/// </summary>
		/// <returns></returns>
		Datum& GetActions();

		/// <summary>
		/// Create and add an action to the given action list
		/// </summary>
		/// <param name="className">Class name of the Action</param>
		/// <param name="instanceName">Instance name of the action</param>
		/// <returns>A Abstract base Action pointer to the newly created Action, Action*</returns>
		Action* CreateAction(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Append an existing Action to the already existing list of Actions
		/// </summary>
		/// <param name="action"></param>
		void AppendAction(Action& action);

		/// <summary>
		/// Call Update on Actions currently in the Action list
		/// </summary>
		/// <param name="worldState"></param>
		void Update(WorldState& worldState);

		static Vector<Signature> GetSignature();

	protected:
		explicit ActionList(const uint64_t& runtimeTypeID, const std::string& name);
		explicit ActionList(const uint64_t& runtimeTypeID);

	private:
		void InitializeSignatures();
	};
}

