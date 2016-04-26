#ifndef _NPC_H_
#define _NPC_H_

#include "IItemInteractable.h"
#include "iTalkNPC.h"
#include <functional>
#include "Entity.h"
#include <string>

#include <map>

#pragma once
class NPC : public Entity, public iTalkNPC, public IItemInteractable
{
private :



public:

	std::string itemResolving = "";

	static inline NPC* _createNPC() {
		return new NPC();
	}

	int solvingRow, solvingColumm;
	std::string keyPass;
	std::string id;
	std::string defaultMessage;

	std::map <std::string, std::function<void*(NPC* npc, std::string keyPass, Room** room)>> unlockMap;
	std::map <std::string, std::string> dialogs; //  every dialog related to the npc is here 
	std::map <std::string, std::string> unlockDialogs; //  special map that stores event dialogs

	NPC();
	NPC(std::string id);
	~NPC();

	void modifyEntity(Italk* talkable, std::string string) override;
	std::string getStatement(std::string key) override;
	std::string getUnlockStatement(std::string key) override;

	void initialize(XMLElement* node);
	std::string getId() override;
	void receiveInteraction(Item* item);
	void setSolvings(int row, int columm, std::string solvingString);
	void unlockDoor(std::string key, Room* room);
	void unlockDoor(std::string key, Room** room);
	static void* unlockDoorWithWord(NPC* npc, std::string key, Room** room);
	bool accessUnlockMap(std::string key, Room** room);
	virtual bool isFightable();

protected:
	
};

#endif