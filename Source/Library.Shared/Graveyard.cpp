#include "pch.h"
#include "Graveyard.h"

namespace FieaGameEngine
{
	void GrimReaper::AddToKillList(Scope& scopeToKill)
	{
		KillList.PushBack(&scopeToKill);
	}

	void GrimReaper::KillAll()
	{
		auto It = KillList.begin();
		for (auto& value : KillList)
		{
			delete value;
		}
		KillList.Clear();
	}

	uint32_t GrimReaper::KillListSize()
	{
		return KillList.Length();
	}

	Vector<Scope*> GrimReaper::KillList;
}
