#include "pch.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"
#include "Action.h"
#include "AbstractFactory.h"
#include "ConcreteFactory.h"
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

		Datum& actions = GetActions();
		for (uint32_t i = 0; i < actions.Length(); ++i)
		{
			assert(actions.Get<Scope*>(i)->Is(Action::TypeIdClass()));
			static_cast<Action*>(actions.Get<Scope*>(i))->Update(worldState);
		}
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

	Datum& Entity::GetActions()
	{
		return (*this)["Actions"];
	}

	Action* Entity::CreateAction(const std::string& actionClassName, const std::string& actionInstanceName)
	{
		Action* newAction = AbstractFactory<Action>::Create(actionClassName);

		newAction->SetName(actionInstanceName);
		newAction->SetEntity(*this);

		return newAction;
	}

	void Entity::Notify(EventPublisher* event)
	{
		UNREFERENCED_PARAMETER(event);
	}

	void Entity::InitializeSignatures()
	{
		AddExternalAttribute("Name", &Name, 1);
		AddNestedScope("Actions");
	}

	void Entity::UpdateExternalStorage()
	{
		UpdateStorage("Name", &Name, 1);
	}
}
