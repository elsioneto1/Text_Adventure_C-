#ifndef _FIGHTABLENPC_H_
#define _FIGHTABLENPC_H_

#include "NPC.h"
#include "IAttack.h"

#pragma once
class Room;
class Fightable_NPC : public NPC , public IAttack
{
	Room* room;
	Room** rooms;
	
public:
	int combatLevel = 0;
	static inline Fightable_NPC* _createFNPC() {
		return new Fightable_NPC();
	}

	int  getCombatLevel(){ return combatLevel; }
	void setCombatLevel(int lvl){ combatLevel = lvl; }
	
	bool isFightable();
	std::string getId() override;
	bool receiveAttack(Player* p) override;
	void setRoomReference(Room * r);

};

#endif