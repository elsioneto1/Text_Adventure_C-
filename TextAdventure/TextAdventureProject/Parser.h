#ifndef _PARSER_H_
#define _PARSER_H_

#include <iostream>
#include <list>

#pragma once
class Parser
{
public:
	static Parser& Instance()
	{
		if (instance == NULL)
		{
			instance = new Parser();
		}
		return *instance;
	}

	void parseCommand(std::string str, std::list<std::string> &list);
	std::string setToUpper(std::string str);
	std::string joinString(std::list<std::string> commands);


private:
	static Parser* instance;

};

#endif