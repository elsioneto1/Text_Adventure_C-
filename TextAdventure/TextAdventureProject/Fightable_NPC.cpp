#include "Fightable_NPC.h"
#include "LevelUpManager.h"
#include "Player.h"
#include "Room.h"

bool Fightable_NPC::isFightable()
{
	unlockMap["OPEN"] = unlockDoorWithWord;
	return true;
}

std::string Fightable_NPC::getId()
{
	return id;
}

bool Fightable_NPC::receiveAttack(Player* p)
{
	int i = 10;
	if (combatLevel >= LevelUpManager::Instance().combatLevel)
	{
		std::cout << getStatement("lose") << std::endl;
	}
	else
	{
		return true;
		//unlockDoor("lostcombat", room);

	}

	return false;
	//return id;
}


void Fightable_NPC::setRoomReference(Room* r)
{
	room = r;
}