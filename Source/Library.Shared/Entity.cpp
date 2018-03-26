#include "pch.h"
#include "Entity.h"

namespace FieaGameEngine
{
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

	void Entity::Update()
	{

	}

	Sector * Entity::GetSector()
	{
		return CurrentSector;
	}

	void Entity::SetSector(Sector & sector)
	{
		CurrentSector = &sector;
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
