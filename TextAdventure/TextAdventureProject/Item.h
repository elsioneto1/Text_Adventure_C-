#ifndef _ITEM_H_
#define _ITEM_H_

#include <string>


#pragma once
class Item
{
	
public:

	

	static inline Item* _Item() {
		return new Item();
}
	std::string id;

	Item();
	~Item();

	virtual bool pickable();

};

#endif