#ifndef ITEM_PARSER_H
#define ITEM_PARSER_H
#include <string>
#include "item.h"
#include "datastore.h"

class ItemParser
{
public:
	ItemParser();
	~ItemParser();
	bool parse(DataStore& ds, std::string imgDir, 
				std::string spriteDir, std::string iconDir);

private:

};

#endif