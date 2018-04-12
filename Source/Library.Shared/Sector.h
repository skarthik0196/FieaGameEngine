#pragma once
#include "Attributed.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	class World;
	class Entity;

	/// <summary>
	/// Sector derived from Attributed
	/// </summary>
	class Sector final : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed)

	public:
		/// <summary>
		/// Default Constructor
		/// </summary>
		Sector();

		/// <summary>
		/// Default constructor that takes in a string
		/// </summary>
		/// <param name="name"></param>
		explicit Sector(const std::string& name);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs"></param>
		Sector(const Sector& rhs);

		/// <summary>
		/// Move Constructor
		/// </summary>
		/// <param name="rhs"></param>
		Sector(Sector&& rhs);

		/// <summary>
		/// Destructor
		/// </summary>
		~Sector();

		/// <summary>
		/// Copy Assignment operator
		/// </summary>
		/// <param name="rhs">Const reference to rhs</param>
		/// <returns>A Sector reference</returns>
		Sector& operator=(const Sector& rhs);

		/// <summary>
		/// Move Assignment operator
		/// </summary>
		/// <param name="rhs">R Value reference to Sector</param>
		/// <returns>Sector reference</returns>
		Sector& operator=(Sector&& rhs);

		/// <summary>
		/// Get Instance name of the sector
		/// </summary>
		/// <returns>A string containing the instance name</returns>
		const std::string& GetName() const;

		/// <summary>
		/// Set new instance name of the sector
		/// </summary>
		/// <param name="name"></param>
		void SetName(const std::string& name);

		/// <summary>
		/// Get world that the sector belongs too
		/// </summary>
		/// <returns></returns>
		World* GetWorld() const;

		/// <summary>
		/// Set World that the sector belongs too
		/// </summary>
		/// <param name="world">World reference</param>
		void SetWorld(World& world);

		/// <summary>
		/// Create an Entity with specified class name and instance name and adds it to Entity list
		/// </summary>
		/// <param name="className">Class name as string</param>
		/// <param name="instanceName">Instance name as string</param>
		/// <returns>An Entity*</returns>
		Entity* CreateEntity(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Update Method
		/// </summary>
		/// <param name="worldState">Refernce to World State</param>
		void Update(WorldState& worldState);

		/// <summary>
		/// Get all Entities present in the Scope
		/// </summary>
		/// <returns>A Datum containing all</returns>
		Datum& GetEntities();

		static Vector<Signature> GetSignature();

		virtual Scope* Clone() const;
		

	private:
		void InitializeSignatures();
		void UpdateExternalStorage();

		std::string Name;
	};
}

