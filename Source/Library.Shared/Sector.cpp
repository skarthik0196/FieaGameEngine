#include "pch.h"
#include "Sector.h"
#include "AbstractFactory.h"
#include "ConcreteFactory.h"
#include "World.h"
#include "Entity.h"
#include <cassert>

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Sector)

	Sector::Sector() : Attributed()
	{
		InitializeSignatures();
	}

	Sector::Sector(std::string& name) : Attributed(), Name(name)
	{
		InitializeSignatures();
	}

	Sector::Sector(const Sector& rhs) : Attributed(rhs), Name(rhs.Name)
	{
		UpdateExternalStorage();
	}

	Sector::Sector(Sector&& rhs) : Attributed(std::move(rhs)), Name(std::move(rhs.Name))
	{
		UpdateExternalStorage();
	}

	Sector::~Sector()
	{

	}

	Sector& Sector::operator=(const Sector& rhs)
	{
		if (this != &rhs)
		{
			Attributed::operator=(rhs);
			Name = rhs.Name;
			UpdateExternalStorage();
		}

		return *this;
	}

	Sector& Sector::operator=(Sector&& rhs)
	{
		if (this != &rhs)
		{
			Attributed::operator=(std::move(rhs));
			Name = std::move(rhs.Name);
			UpdateExternalStorage();
		}

		return *this;
	}

	const std::string& Sector::GetName() const
	{
		return Name;
	}

	void Sector::SetName(const std::string& name)
	{
		Name = name;
	}

	World* Sector::GetWorld() const
	{
		return CurrentWorld;
	}

	void Sector::SetWorld(World& world)
	{
		CurrentWorld = &world;
	}

	Entity* Sector::CreateEntity(const std::string & className, const std::string & instanceName)
	{
		Entity *newEntity = AbstractFactory<RTTI>::Create(className)->As<Entity>();
		
		newEntity->SetName(instanceName);
		newEntity->SetSector(*this);

		this->Adopt(*newEntity, "Entities");

		return newEntity;
	}

	void Sector::Update(WorldState& worldState)
	{
		Datum& EntityList = (*this)["Entities"];
		worldState.CurrentSector = this;

		for (uint32_t i = 0; i < EntityList.Length(); ++i)
		{
			assert(EntityList.Get<Scope*>(i)->Is("Entity"));
			static_cast<Entity*>(EntityList.Get<Scope*>(i))->Update(worldState);
		}
	}

	Datum& Sector::GetEntities()
	{
		return (*this)["Entities"];
	}

	void Sector::InitializeSignatures()
	{
		AddExternalAttribute("Name", &Name, 1);
		AddNestedScope("Entities");
	}

	void Sector::UpdateExternalStorage()
	{
		UpdateStorage("Name", &Name, 1);
	}
}
