#ifndef _ITALKNPC_H_
#define _ITALKNPC_H_

#pragma once
#include "Italk.h"
#include <string>
class Room;
class iTalkNPC :
	public Italk
{
public:

	virtual std::string getStatement(std::string key) = 0;
	virtual std::string getUnlockStatement(std::string key) = 0;
	virtual bool accessUnlockMap(std::string key, Room** room) = 0;

};

#endif