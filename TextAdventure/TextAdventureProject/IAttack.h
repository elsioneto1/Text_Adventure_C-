#ifndef _IATTACK_H_
#define _IATTACK_H_

#include <string>
class Player;
#pragma once
class IAttack
{
public:


	virtual std::string getId() = 0;
	virtual bool receiveAttack(Player* p) = 0;
	//virtual std::string getStatement(std::string key) = 0;

};

#endif