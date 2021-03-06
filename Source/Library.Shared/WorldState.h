#pragma once
#include "GameClock.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	class World;
	class Sector;
	class Entity;
	class Action;

	/// <summary>
	/// The World State class that holds pointers to all the current objects being processed
	/// </summary>
	class WorldState
	{
	public:
		/// <summary>
		/// Default Constructor
		/// </summary>
		WorldState() = default;

		/// <summary>
		/// Destructor
		/// </summary>
		~WorldState() = default;

		/// <summary>
		/// Public Variables
		/// </summary>
		World* CurrentWorld{ nullptr };
		Sector* CurrentSector{ nullptr };
		Entity* CurrentEntity{ nullptr };
		Action* CurrentAction{ nullptr };

		/// <summary>
		/// Update Method to update GameTime
		/// </summary>
		void Update();

		/// <summary>
		/// Set the Game Time
		/// </summary>
		/// <param name="gameTime">GameTime reference</param>
		void SetGameTime(GameTime& gameTime);

		/// <summary>
		/// Get the GameTime
		/// </summary>
		/// <returns>A GameTime refernece</returns>
		GameTime& GetGameTime();

		/// <summary>
		/// Get Game Clock
		/// </summary>
		/// <returns>A GameClock reference</returns>
		GameClock& GetGameClock();

	private:
		GameTime GTime;
		GameClock GClock;
	};
}

