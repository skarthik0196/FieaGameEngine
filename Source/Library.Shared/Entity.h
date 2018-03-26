#pragma once
#include "Attributed.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	class Sector;
	class World;

	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed)

	public:
		Entity();
		explicit Entity(const std::string& name);
		virtual ~Entity();
		Entity(const Entity& rhs);
		Entity(Entity&& rhs);

		Entity& operator=(const Entity& rhs);
		Entity& operator=(Entity&& rhs);


		void SetName(const std::string& name);
		const std::string& GetName();

		void Update(WorldState& worldState);
		Sector* GetSector();
		void SetSector(Sector& sector);

	private:
		void InitializeSignatures();
		void UpdateExternalStorage();

		std::string Name;
	};
}

