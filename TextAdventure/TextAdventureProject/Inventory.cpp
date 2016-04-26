#include "Inventory.h"
#include "LevelUpManager.h"
#include <iostream>


Inventory::Inventory()
{
}


Inventory::~Inventory()
{
}

void Inventory::AddItem(Item* item)
{
	itemMap[item->id] = item;
}

Item* Inventory::GetItem(std::string itemKey)
{
	//Item* i = NULL; // = itemMap[itemKey];
	std::map<std::string, Item*>::iterator it;
	for (it = itemMap.begin(); it != itemMap.end(); ++it)
	{
		if (it->first == itemKey)
		{
			return it->second;
		}
	}
	return NULL;
}

void Inventory::RemoveItem(Item* item)
{

}

void Inventory::showInventory()
{

	std::cout << "Inventory: "  << std::endl;
	std::map<std::string, Item*>::iterator it;
	for (it = itemMap.begin(); it != itemMap.end(); ++it)
	{
		std::cout << "You got " << it->first << std::endl;
	}

	std::cout << "Levels: " << std::endl;
	LevelUpManager::Instance().printLevels();
}

void Inventory::clearItems()
{
	// clears menu
	std::map<std::string, Item*>::iterator inventoryIt;
	for (inventoryIt = itemMap.begin(); inventoryIt != itemMap.end(); ++inventoryIt)
	{
		delete inventoryIt->second;
	}
	itemMap.clear();

}