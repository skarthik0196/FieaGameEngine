#include "pch.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"
#include <cassert>

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Entity)

	Entity::Entity() : Attributed()
	{
		InitializeSignatures();
	}

	Entity::Entity(const std::string& name) :Attributed(), Name(name)
	{
		InitializeSignatures();
	}

	Entity::Entity(const Entity& rhs) : Attributed(rhs), Name(rhs.Name)
	{
		UpdateExternalStorage();
	}

	Entity::Entity(Entity&& rhs) : Attributed(std::move(rhs)), Name(std::move(rhs.Name))
	{
		UpdateExternalStorage();
	}

	Entity& Entity::operator=(const Entity& rhs)
	{
		if (this != &rhs)
		{
			Attributed::operator=(rhs);
			Name = rhs.Name;
			UpdateExternalStorage();
		}
		return *this;
	}

	Entity& Entity::operator=(Entity&& rhs)
	{
		if (this != &rhs)
		{
			Attributed::operator=(std::move(rhs));
			Name = std::move(rhs.Name);
			UpdateExternalStorage();
		}
		return *this;
	}


	Entity::~Entity()
	{
	}

	void Entity::SetName(const std::string & name)
	{
		Name = name;
	}

	const std::string & Entity::GetName()
	{
		return Name;
	}

	void Entity::Update(WorldState& worldState)
	{
		worldState.CurrentEntity = this;
	}

	Sector* Entity::GetSector()
	{
		assert(GetParent()->Is("Sector"));
		return static_cast<Sector*>(GetParent());
	}

	void Entity::SetSector(Sector& sector)
	{
		sector.Adopt(*this, "Entities");
	}

	void Entity::InitializeSignatures()
	{
		AddExternalAttribute("Name", &Name, 1);
	}

	void Entity::UpdateExternalStorage()
	{
		UpdateStorage("Name", &Name, 1);
	}
}
