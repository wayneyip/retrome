#ifndef ITEM_PARSER_H
#define ITEM_PARSER_H
#include <string>
#include "item.h"
#include "layereditem.h"
#include "datastore.h"

class ItemParser
{
public:
	ItemParser();
	~ItemParser();
	bool parseCategories(DataStore& ds, std::string categoryFile);
	bool parseColors(DataStore& ds, std::string colors, std::string category);
	bool parseItems(DataStore& ds, std::string imgDir, 
				std::string spriteDir, std::string iconDir);

private:
	enum PState { READTYPE, READCATEGORY, READCOLORS };
	bool error_;
};

#endif