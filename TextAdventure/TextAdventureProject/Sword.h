#ifndef _SWORD_H_
#define _SWORD_H_

#include "IAttack.h"

#pragma once
class Sword
{
public:
	Sword();
	~Sword();
	virtual void attack(IAttack* attack);

};

#endif