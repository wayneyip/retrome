#ifndef DATASTORE_H
#define DATASTORE_H

#include <map>
#include <vector>
#include <string>
#include <queue>
#include "item.h"

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
	void addItem(Item* item);
	void selectItem(Item* item);
	Item* findEquippedItem(std::string type, std::string category);
	equippedItemHeap getAllEquippedItems();
	std::vector<std::string> getTypeCategories(std::string type);
	std::vector<Item*> getCategoryItems(std::string type, std::string category);

private:
	// Debug functions
	void printItemMap();
	void printAvatarMap();

	// Map of item types to categories to items
	// (e.g. "body" to "hair" to vector<Hair*>)
	std::map<std::string, std::map<std::string, std::vector<Item*> > > typeCategoryItemMap_;
	
	// Map of avatar's body parts to currently equipped items
	// (e.g. "hair" to a particular Hair*)
	std::map<std::string, Item*> avatarItemMap_;
};

#endif