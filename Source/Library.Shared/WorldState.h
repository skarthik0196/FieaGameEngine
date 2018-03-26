#pragma once
#include "GameClock.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	class World;
	class Sector;
	class Entity;

	class WorldState
	{
	public:
		WorldState() = default;
		~WorldState() = default;

		World* CurrentWorld{ nullptr };
		Sector* CurrentSector{ nullptr };
		Entity* CurrentEntity{ nullptr };
		void Update();

		void SetGameTime(GameTime& gameTime);
		GameTime& GetGameTime();

	private:
		GameTime GTime;
		GameClock GClock;
	};
}

