#pragma once
#include "Attributed.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	class Entity;

	class Action : public Attributed
	{
	public:
		Action();
		Action(const std::string& name);
		Action(const Action& rhs);
		Action(Action&& rhs);
		Action& operator=(const Action& rhs);
		Action& operator=(Action&& rhs);
		virtual ~Action();

		const std::string& GetName() const;
		void SetName(const std::string& name);
		virtual void Update(WorldState& worldState) = 0;

		Entity* GetEntity();
		void SetEntity(Entity& entity);

	private:
		void InitializeSignatures();
		void UpdateExternalStorage();
		std::string Name;
	};
}