#pragma once
#include "Attributed.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	class Sector;
	class Entity;

	/// <summary>
	/// World class that derives from Attributed
	/// </summary>
	class World final : public Attributed
	{
	RTTI_DECLARATIONS(World, Attributed)

	public:
		/// <summary>
		/// Default Constructor
		/// </summary>
		World();

		/// <summary>
		/// Default constructor for initializing with instance name
		/// </summary>
		/// <param name="name">const reference to instance name</param>
		explicit World(const std::string& name);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">Const reference to world</param>
		World(const World& rhs);

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs"></param>
		World(World&& rhs);

		/// <summary>
		/// Destructor
		/// </summary>
		~World();

		/// <summary>
		/// Copy Assignment operator
		/// </summary>
		/// <param name="rhs">Const world reference</param>
		/// <returns>A world reference</returns>
		World& operator=(const World& rhs);

		/// <summary>
		/// Move Assignment Operator
		/// </summary>
		/// <param name="rhs">R value reference to a World</param>
		/// <returns>A world reference</returns>
		World& operator=(World&& rhs);

		/// <summary>
		/// Returns the instance name of the World
		/// </summary>
		/// <returns>A string reference to the instance name</returns>
		const std::string& GetName() const;

		/// <summary>
		/// Set the instance name of the world
		/// </summary>
		/// <param name="name">A const reference to the name</param>
		void SetName(const std::string& name);

		/// <summary>
		/// Update method
		/// </summary>
		void Update();

		/// <summary>
		/// Creates a Sector and adds it to the current world
		/// </summary>
		/// <param name="instanceName">The instance name of the string</param>
		/// <returns>A Sector*</returns>
		Sector* CreateSector(const std::string& instanceName);

		/// <summary>
		/// Get World State
		/// </summary>
		/// <returns>The world state object</returns>
		WorldState& GetWorldState();

		/// <summary>
		/// Get the list of all sectors in the World
		/// </summary>
		/// <returns>A datum reference to the datum containing all sectors in the world</returns>
		Datum& GetSectors();
	private:
		void InitializeSignatures();
		void UpdateExternalStorage();

		std::string Name;
		WorldState CurrentWorldState;
	};
}

