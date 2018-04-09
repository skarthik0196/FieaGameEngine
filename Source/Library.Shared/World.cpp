#include "pch.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "Graveyard.h"
#include<cassert>

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(World)

	World::World() : Attributed(TypeIdInstance())
	{
		//InitializeSignatures();
	}

	World::World(const std::string& name) : Attributed(TypeIdInstance()), Name(name)
	{
		//InitializeSignatures();
	}

	World::World(const World& rhs) : Attributed(rhs), Name(rhs.Name)
	{
		//UpdateExternalStorage();
	}

	World::World(World&& rhs) : Attributed(std::move(rhs)), Name(std::move(rhs.Name))
	{
		//UpdateExternalStorage();
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
			//UpdateExternalStorage();
		}
		return *this;
	 }

	World& World::operator=(World&& rhs)
	{
		if (this != &rhs)
		{
			Attributed::operator=(std::move(rhs));
			Name = std::move(rhs.Name);
			//UpdateExternalStorage();
		}

		return *this;
	}

	const std::string& World::GetName() const
	{
		return Name;
	}
	void World::SetName(const std::string& name)
	{
		Name = name;
	}

	void World::Update()
	{
		CurrentWorldState.CurrentWorld = this;
		CurrentWorldState.Update();
		GrimReaper::KillAll();
		WorldEventQueue.Update(CurrentWorldState.GetGameTime());

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
		newSector->SetName(instanceName);
		newSector->SetWorld(*this);

		return newSector;
	}

	WorldState& World::GetWorldState()
	{
		return CurrentWorldState;
	}

	Datum& World::GetSectors()
	{
		return (*this)["Sectors"];
	}

	EventQueue & World::GetEventQueue()
	{
		return WorldEventQueue;
	}

	Vector<Signature> World::GetSignature()
	{
		Vector<Signature> signature;
		signature.PushBack(Signature("Name", Datum::DatumType::String, offsetof(World, Name), 1));
		signature.PushBack(Signature("Sectors", Datum::DatumType::Table, offsetof(World, Name), 1));

		return signature;
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
