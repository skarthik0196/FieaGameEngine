#pragma once
#include "Attributed.h"
#include "WorldState.h"
#include "EventSubscriber.h"

namespace FieaGameEngine
{
	class Sector;
	class World;
	class Action;

	/// <summary>
	/// Entity class derived from Attributed
	/// </summary>
	class Entity : public Attributed, public EventSubscriber
	{
		RTTI_DECLARATIONS(Entity, Attributed)

	public:
		/// <summary>
		/// Default constructor that initializes attributes
		/// </summary>
		Entity();

		/// <summary>
		/// Parameterized constructor for initializing with name
		/// </summary>
		/// <param name="name">Instance Name of the entity</param>
		explicit Entity(const std::string& name);

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~Entity();

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Const reference to enety</param>
		Entity(const Entity& rhs);

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs">R value reference to Entity</param>
		Entity(Entity&& rhs);

		/// <summary>
		/// Copy Assignment operator
		/// </summary>
		/// <param name="rhs">Const reference to Entity</param>
		/// <returns>An Entity Reference</returns>
		Entity& operator=(const Entity& rhs);

		/// <summary>
		/// Move Assignment operator
		/// </summary>
		/// <param name="rhs">R value reference to entity</param>
		/// <returns>An Entity reference</returns>
		Entity& operator=(Entity&& rhs);

		/// <summary>
		/// Set name of the Entity
		/// </summary>
		/// <param name="name">const reference to the name</param>
		void SetName(const std::string& name);

		/// <summary>
		/// Gets the name of the entity
		/// </summary>
		/// <returns>A string reference to the name of the entity</returns>
		const std::string& GetName();

		/// <summary>
		/// Update method
		/// </summary>
		/// <param name="worldState">A reference to the WorldState</param>
		virtual void Update(WorldState& worldState);

		/// <summary>
		/// Get sector in which the Entity is present
		/// </summary>
		/// <returns>A sector /</returns>
		Sector* GetSector();

		/// <summary>
		/// Set the sector in which the Entity is present
		/// </summary>
		/// <param name="sector">A sector&</param>
		void SetSector(Sector& sector);

		Datum& GetActions();

		Action* CreateAction(const std::string& actionClassName, const std::string& actionInstanceName);

		void Notify(EventPublisher* event);

		static Vector<Signature> GetSignature();

	protected:
		Entity(const uint64_t& runtimeTypeID);
		Entity(const uint64_t& runtimeTypeID, const std::string& name);

	private:
		void InitializeSignatures();
		void UpdateExternalStorage();

		std::string Name;
	};
}

