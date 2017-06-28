#ifndef DATASTORE_H
#define DATASTORE_H

#include <map>
#include <vector>
#include <string>
#include "item.h"

class DataStore
{
public:
	typedef std::map<std::string, std::map<std::string, std::vector<Item*> > > typeMap;
	typedef std::map<std::string, std::vector<Item*> > categoryMap;

	DataStore();
	~DataStore();
	void addItem(Item* item);
	std::vector<std::string> getTypeCategories(std::string type);
	std::vector<Item*> getCategoryItems(std::string type, std::string category);

	void printMap();

private:
	// Map of item types to categories to items
	// (i.e. "body" to "eyes" to vector<Eyes*>)
	std::map<std::string, std::map<std::string, std::vector<Item*> > > typeCategoryItemMap_;
	
	// Map of avatar's body parts to currently equipped items
	std::map<std::string, Item*> avatarItemMap_;
};

#endif