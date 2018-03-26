#include "pch.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include<cassert>

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(World)

	World::World() : Attributed()
	{
		InitializeSignatures();
	}

	World::World(const std::string& name) : Attributed(), Name(name)
	{
		InitializeSignatures();
	}

	World::World(const World& rhs) : Attributed(rhs), Name(rhs.Name)
	{
		UpdateExternalStorage();
	}

	World::World(World&& rhs) : Attributed(std::move(rhs)), Name(std::move(rhs.Name))
	{
		UpdateExternalStorage();
	}

	World::~World()
	{

	}

	World& World::operator=(const World& rhs)
	{
		if (this != &rhs)
		{
			Attributed::operator=(rhs);
			Name = rhs.Name;
			UpdateExternalStorage();
		}
		return *this;
	 }

	World& World::operator=(World&& rhs)
	{
		if (this != &rhs)
		{
			Attributed::operator=(std::move(rhs));
			Name = std::move(rhs.Name);
			UpdateExternalStorage();
		}

		return *this;
	}

	const std::string& World::GetName() const
	{
		return Name;
	}
	void World::SetName(const std::string & name)
	{
		Name = name;
	}

	void World::Update()
	{
		CurrentWorldState.CurrentWorld = this;
		CurrentWorldState.Update();

		Datum& sectorList = (*this)["Sectors"];
		for (uint32_t i = 0; i < sectorList.Length(); ++i)
		{
			assert(sectorList.Get<Scope*>(i)->Is("Sector"));
			static_cast<Sector*>(sectorList.Get<Scope*>(i))->Update(CurrentWorldState);
		}
	}

	Sector * World::CreateSector(const std::string& instanceName)
	{
		Sector *newSector = new Sector();
		Adopt(*newSector, "Sectors");
		newSector->SetName(instanceName);
		newSector->SetWorld(*this);

		return newSector;
	}

	const WorldState& World::GetWorldState() const
	{
		return CurrentWorldState;
	}

	Datum & World::GetSectors()
	{
		return (*this)["Sectors"];
	}

	void World::InitializeSignatures()
	{
		AddExternalAttribute("Name", &Name, 1);
		AddNestedScope("Sectors");
	}

	void World::UpdateExternalStorage()
	{
		UpdateStorage("Name", &Name, 1);
	}
}
