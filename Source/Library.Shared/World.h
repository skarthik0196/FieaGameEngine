#pragma once
#include "Attributed.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	class Sector;
	class Entity;

	class World final : public Attributed
	{
	RTTI_DECLARATIONS(World, Attributed)

	public:
		World();
		explicit World(const std::string& name);
		World(const World& rhs);
		World(World&& rhs);
		~World();

		World& operator=(const World& rhs);
		World& operator=(World&& rhs);

		const std::string& GetName() const;
		void SetName(const std::string& name);

		void Update();
		Sector* CreateSector(const std::string& instanceName);

		const WorldState& GetWorldState() const;

		Datum& GetSectors();
	private:
		void InitializeSignatures();
		void UpdateExternalStorage();

		std::string Name;
		WorldState CurrentWorldState;
	};
}

