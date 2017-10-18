#ifndef DATASTORE_H
#define DATASTORE_H

#include <map>
#include <vector>
#include <string>
#include <queue>
#include "item.h"
#include "layereditem.h"
#include "color.h"

struct equipPriority
{
	bool operator()(const Item* lhs, const Item* rhs) const
	{
		return lhs->getPriority() > rhs->getPriority();
	}
};

class DataStore
{
public:
	typedef std::map<std::string, std::map<std::string, std::vector<Item*> > > typeMap;
	typedef std::map<std::string, std::vector<Item*> > categoryMap;
	typedef std::map<std::string, Item*> avatarMap;
	typedef std::priority_queue<Item*, std::vector<Item*>, struct equipPriority> equippedItemHeap;

	DataStore();
	~DataStore();

	bool addType(std::string type);
	bool addCategory(std::string type, std::string category);
	bool addColor(Color* color);
	bool addItem(Item* item);

	std::vector<std::string> getTypeCategories(std::string type);
	std::string getCategoryType(std::string category);

	std::vector<Item*> getCategoryItems(std::string category);
	void selectItem(Item* item);
	Item* findEquippedItem(std::string category);
	equippedItemHeap getAllEquippedItems();
	void selectRandomItems();

	std::vector<Color*> getCategoryColors(std::string category);
	void changeColor(Color* color);
	Color* findSelectedColor(std::string category);

	// Debug functions
	void printItemMap();
	void printAvatarMap();
	void PRINT(std::string statement);

private:

	// Map of item types to categories to items
	// (e.g. "body" to "hair" to vector<Item*>)
	std::map<std::string, std::map<std::string, std::vector<Item*> > > typeCategoryItemMap_;
	
	// Map of avatar's body parts to currently equipped items
	// (e.g. "hair" to a particular Item* with hair sprite & icon)
	std::map<std::string, Item*> avatarItemMap_;

	// Map of categories to types
	// (e.g. "hair" to "body")
	std::map<std::string, std::string> categoryTypeMap_;

	// Map of item types to categories to list of available colors
	// (e.g. "body" to hair" to black, brown, blonde)
	std::map<std::string, std::map<std::string, std::vector<Color*> > > typeCategoryColorMap_;

	// Map of avatar's body parts to currently selected colors
	// (e.g. "hair" to black)
	std::map<std::string, Color*> avatarColorMap_;
};

#endif