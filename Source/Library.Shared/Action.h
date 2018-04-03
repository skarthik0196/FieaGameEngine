#pragma once
#include "Attributed.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	class Entity;

	/// <summary>
	/// Base Action class that derives from Attributed
	/// </summary>
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed)

	public:

		/// <summary>
		/// Default Constructor
		/// </summary>
		Action();

		/// <summary>
		/// Default constructor that takes a string reference
		/// </summary>
		/// <param name="name">Name of the Action</param>
		Action(const std::string& name);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Reference to another action</param>
		Action(const Action& rhs);

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">R value reference to action</param>
		Action(Action&& rhs);

		/// <summary>
		/// Copy Assignment operator
		/// </summary>
		/// <param name="rhs">Reference to another action</param>
		/// <returns></returns>
		Action& operator=(const Action& rhs);

		/// <summary>
		/// Move Assignment operator
		/// </summary>
		/// <param name="rhs">R value reference to an action</param>
		/// <returns></returns>
		Action& operator=(Action&& rhs);
		virtual ~Action();

		/// <summary>
		/// Get name of action
		/// </summary>
		/// <returns>A string containing name of the action</returns>
		const std::string& GetName() const;

		/// <summary>
		/// Set the name of the action
		/// </summary>
		/// <param name="name">String containing the new name of the action</param>
		void SetName(const std::string& name);
		
		/// <summary>
		/// Pure virtual update method
		/// </summary>
		/// <param name="worldState">World State reference</param>
		virtual void Update(WorldState& worldState) = 0;

		/// <summary>
		/// Get Parent Entity
		/// </summary>
		/// <returns>Pointer to parent Entity</returns>
		Entity* GetEntity();

		/// <summary>
		/// Set Parent entity
		/// </summary>
		/// <param name="entity">Reference to an entity</param>
		void SetEntity(Entity& entity);

	private:
		void InitializeSignatures();
		void UpdateExternalStorage();
		std::string Name;
	};
}