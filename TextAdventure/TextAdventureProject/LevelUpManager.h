#ifndef _LEVELUPMANAGER_H_
#define _LEVELUPMANAGER_H_

#include <string>
#include <list>
#include <iostream>	
#include "iTalkNPC.h"

#pragma once
class LevelUpManager
{

	std::list<std::string> speakLevelUpList;
	std::list<std::string> moveLevelUpList;
	std::list<std::string> combatLevelUpList;
	
public:
	int speakLevel	= 1;
	int moveLevel	= 0;
	int combatLevel = 1;
	//typedef void increment(std::string npc);

	static inline void *_increaseSpeak(std::string npc, iTalkNPC* italk)
	{
		std::string s = "";
		std::list<std::string>::iterator it;
		for (it = LevelUpManager::Instance().speakLevelUpList.begin();
			it != LevelUpManager::Instance().speakLevelUpList.end();
			++it)
		{
			if (*it == npc)
			{
				s = npc;
			}
		}

		if (s == "")
		{
			LevelUpManager::Instance().speakLevel++;
			LevelUpManager::Instance().speakLevelUpList.push_back(npc);

			std::cout << "Your speak level raised from " << LevelUpManager::Instance().speakLevel - 1 << " to" << LevelUpManager::Instance().speakLevel << std::endl;
			std::cout << italk->getStatement("raise");
		}
		else
		{
			// if this npc already lvlup the player, he'll get a personalized message telling that he is already lvl up
			if ( italk != NULL)
				std::cout << italk->getStatement("raised") << std::endl;
			
		}

		
		return NULL;
	}


	static inline void *_increaseMove(std::string npc, iTalkNPC* italk)
	{
		std::string s = "";
		std::list<std::string>::iterator it;
		for (it = LevelUpManager::Instance().moveLevelUpList.begin();
			it != LevelUpManager::Instance().moveLevelUpList.end();
			++it)
		{
			if (*it == npc)
			{
				s = npc;
			}
		}

		if (s == "")
		{
			LevelUpManager::Instance().moveLevel++;
			LevelUpManager::Instance().moveLevelUpList.push_back(npc);
			std::cout << "Your move level raised from " << LevelUpManager::Instance().moveLevel - 1 << " to" << LevelUpManager::Instance().moveLevel << std::endl;
			std::cout << italk->getStatement("raise");
		}
		else
		{
			// if this npc already lvlup the player, he'll get a personalized message telling that he is already lvl up
			if (italk != NULL)
				std::cout << italk->getStatement("raised") << std::endl;
		
		}


		return NULL;
	}

	static inline void *_increaseCombat(std::string npc, iTalkNPC* italk)
	{
		std::string s = "";
		std::list<std::string>::iterator it;
		for (it = LevelUpManager::Instance().combatLevelUpList.begin();
			it != LevelUpManager::Instance().combatLevelUpList.end();
			++it)
		{
			if (*it == npc)
			{
				s = npc;
			}
		}

		if (s == "")
		{
			LevelUpManager::Instance().combatLevel++;
			LevelUpManager::Instance().combatLevelUpList.push_back(npc);
			std::cout << "Your combat level raised from " << LevelUpManager::Instance().combatLevel - 1 << " to " << LevelUpManager::Instance().combatLevel << std::endl;
			std::cout << italk->getStatement("raise");
		}
		else
		{
			// if this npc already lvlup the player, he'll get a personalized message telling that he is already lvl up
			if (italk != NULL)
				std::cout << italk->getStatement("raised") << std::endl;
			
				
		}


		return NULL;
	}

	static LevelUpManager& Instance()
	{
		if (instance == NULL)
		{
			instance = new LevelUpManager();
		}
		return *instance;
	}
	
	void printLevels()
	{
		std::cout << "Current Speak level :"	<< LevelUpManager::Instance().speakLevel << std::endl;
		std::cout << "Current Combat level :"	<< LevelUpManager::Instance().combatLevel << std::endl;
		std::cout << "Current Move level :"		<< LevelUpManager::Instance().moveLevel << std::endl;
	}


private:
	static LevelUpManager* instance;


};

#endif