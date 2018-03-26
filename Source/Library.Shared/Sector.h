#pragma once
#include "Attributed.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	class World;
	class Entity;

	class Sector final : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed)

	public:
		Sector();
		explicit Sector(std::string& name);
		Sector(const Sector& rhs);
		Sector(Sector&& rhs);
		~Sector();

		Sector& operator=(const Sector& rhs);
		Sector& operator=(Sector&& rhs);

		const std::string& GetName() const;
		void SetName(const std::string& name);
		World* GetWorld() const;
		void SetWorld(World& world);

		Entity* CreateEntity(const std::string& className, const std::string& instanceName);
		void Update(WorldState& worldState);
		Datum& GetEntities();

	private:
		void InitializeSignatures();
		void UpdateExternalStorage();

		std::string Name;
		World* CurrentWorld{ nullptr };
	};
}

