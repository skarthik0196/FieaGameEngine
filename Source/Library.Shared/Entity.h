#pragma once
#include "Attributed.h"
#include <string>

namespace FieaGameEngine
{
	class Sector;
	class Entity : public Attributed
	{
	public:
		explicit Entity(const std::string& name);
		virtual ~Entity();
		Entity(const Entity& rhs);
		Entity(Entity&& rhs);

		Entity& operator=(const Entity& rhs);
		Entity& operator=(Entity&& rhs);


		void SetName(const std::string& name);
		void Update();
		Sector* GetSector();
		void SetSector(Sector& sector);

	private:
		void InitializeSignatures();
		void UpdateExternalStorage();

		std::string Name;
		Sector* CurrentSector{ nullptr };
	};
}

