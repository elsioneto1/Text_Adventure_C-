#ifndef _ITALK_H_
#define _ITALK_H_
#include <string>

#pragma once
class Italk
{
public:

	virtual void modifyEntity(Italk* talkable, std::string modifiable) = 0;

	virtual std::string getId() = 0;



};

#endif