#include "pch.h"
#include "WorldState.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"


namespace FieaGameEngine
{
	void WorldState::Update()
	{
		GClock.UpdateGameTime(GTime);
	}

	void WorldState::SetGameTime(GameTime & gameTime)
	{
		GTime = gameTime;
	}

	GameTime& WorldState::GetGameTime()
	{
		return GTime;
	}
	GameClock & WorldState::GetGameClock()
	{
		return GClock;
	}
}
