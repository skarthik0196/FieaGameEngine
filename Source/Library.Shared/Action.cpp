#include "pch.h"
#include "Action.h"
#include "Entity.h"
#include <cassert>

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Action)

	Action::Action(): Attributed(TypeIdInstance())
	{
		//InitializeSignatures();
	}

	Action::Action(const std::string& name) : Attributed(TypeIdInstance()), Name(name)
	{
		//InitializeSignatures();
	}


	Action::Action(const uint64_t& runtimeTypeID) : Attributed(runtimeTypeID)
	{

	}

	Action::Action(const uint64_t& runtimeTypeID, const std::string& name) : Attributed(runtimeTypeID), Name(name)
	{

	}

	Action::Action(const Action& rhs) : Attributed(rhs), Name(rhs.Name)
	{
		//UpdateExternalStorage();
	}

	Action::Action(Action&& rhs) : Attributed(std::move(rhs)), Name(std::move(rhs.Name))
	{
		//UpdateExternalStorage();
	}

	Action& Action::operator=(const Action& rhs)
	{
		if (this != &rhs)
		{
			Attributed::operator=(rhs);
			Name = rhs.Name;
			//UpdateExternalStorage();
		}
		return *this;
	}

	Action & Action::operator=(Action&& rhs)
	{
		if (this != &rhs)
		{
			Attributed::operator=(std::move(rhs));
			Name = std::move(rhs.Name);
			//UpdateExternalStorage();
		}
		return *this;
	}

	Action::~Action()
	{
	}
	const std::string & Action::GetName() const
	{
		return Name;
	}

	void Action::SetName(const std::string& name)
	{
		Name = name;
	}

	Entity * Action::GetEntity()
	{
		assert(GetParent()->Is(Entity::TypeIdClass()));
		return static_cast<Entity*>(GetParent());
	}

	void Action::SetEntity(Entity& entity)
	{
		entity.Adopt(*this, "Actions");
	}

	Vector<Signature> Action::GetSignature()
	{
		Vector<Signature> signature;
		signature.PushBack(Signature("Name", Datum::DatumType::String, offsetof(Action, Name), 1));

		return signature;
	}


	void Action::InitializeSignatures()
	{
		AddExternalAttribute("Name", &Name, 1);
	}

	void Action::UpdateExternalStorage()
	{
		UpdateStorage("Name", &Name, 1);
	}
}
