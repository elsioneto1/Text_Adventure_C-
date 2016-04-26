#include "Parser.h"

Parser* Parser::instance = NULL;


void Parser::parseCommand(std::string str, std::list<std::string> &list)
{

	std::string compareString;
	int index = 0;
	const char *breakStr = " ";
	std::string command = "";
	while (str[index] != NULL)
	{
		
		if (str[index] == *breakStr) // if it finds a blank space, recognizes as it a new command
		{
			list.push_back(command);
			command = "";
		}
		else
		{
			// all comands are converted to upper case, so the player can be interpreted better
			command.push_back(toupper(str[index]));
		}

		index++;

	}
	list.push_back(command);
}

std::string Parser::setToUpper(std::string str)
{
	std::string s;

	int index = 0;
	while (str[index] != NULL)
	{
		s.push_back(toupper(str[index]));
		index++;
	}
	return s;
}

std::string Parser::joinString(std::list<std::string> commands)
{
	std::string s = "";
	std::list<std::string>::iterator it = commands.begin();
	for (it; it != commands.end(); ++it)
	{

		s.append(*it);
		s.append("_");
	}

	return s;
}