#pragma once
#include "Scope.h"

namespace FieaGameEngine
{

	/// <summary>
	/// I reap the souls of all objects who have fallen into depravity
	/// </summary>
	class GrimReaper
	{
	public:

		/// <summary>
		/// He's a supreme being so there's only of him
		/// </summary>
		GrimReaper() = delete;
		GrimReaper(const GrimReaper& rhs) = delete;
		GrimReaper(GrimReaper&& rhs) = delete;
		~GrimReaper() = delete;;

		GrimReaper& operator=(const GrimReaper& rhs) = delete;
		GrimReaper& operator=(GrimReaper&& rhs) = delete;

		/// <summary>
		/// Sucks to be you!
		/// </summary>
		/// <param name="scopeToKill"></param>
		static void AddToKillList(Scope& scopeToKill);

		/// <summary>
		/// I'm coming for you! 
		/// </summary>
		static void KillAll();

		/// <summary>
		/// The never ending list of perpetrators.
		/// </summary>
		/// <returns></returns>
		static uint32_t KillListSize();

	private:
		static Vector<Scope*> KillList;
	};
}

