#include "datastore.h"
#include <algorithm>
#include <iostream>

DataStore::DataStore()
{

}

DataStore::~DataStore()
{

}

void DataStore::addItem(Item* item)
{
	// Register item in typeCategoryItemMap_, under its type and category
	std::string type = item->getType();
	std::string category = item->getCategory();

	// Does item type exist?
	if (typeCategoryItemMap_.find(type) == typeCategoryItemMap_.end())
	{
		// If type does not exist, the category does not exist either
		// Put item in a new vector<Item*>
		std::vector<Item*> itemList;
		itemList.push_back(item);

		// Insert item and its category into a new map 
		std::map<std::string, std::vector<Item*> > categoryItemMap;
		categoryItemMap.insert(std::make_pair(category, itemList));

		// Insert the new map into typeCategoryItemMap_
		typeCategoryItemMap_.insert(std::make_pair(type, categoryItemMap));

		// Also insert the new category into avatarItemMap_
		Item* NULLPTR = NULL;
		avatarItemMap_.insert(std::make_pair(category, NULLPTR));
	}
	else
	{
		// If type exists, check if the category exists
		if (typeCategoryItemMap_[type].find(category) == typeCategoryItemMap_[type].end())
		{
			// If category does not exist,
			// put item in a new vector<Item*>, insert with category
			std::vector<Item*> itemList;
			itemList.push_back(item);
			typeCategoryItemMap_[type].insert(std::make_pair(category, itemList));

			// Also insert the new category into avatarItemMap_
			Item* NULLPTR = NULL;
			avatarItemMap_.insert(std::make_pair(category, NULLPTR));
		}
		else
		{
			// If category exists,
			// find category and just insert item
			typeCategoryItemMap_[type][category].push_back(item);
		}
	}
}

void DataStore::selectItem(Item* item)
{
	if (avatarItemMap_.find(item->getCategory()) == avatarItemMap_.end())
	{
		std::cout << "ERROR: NO CATEGORY FOUND FOR ITEM " + item->getSpriteName() << std::endl;
		return;
	}
	avatarItemMap_[item->getCategory()] = item;
}

Item* DataStore::findEquippedItem(std::string type, std::string category)
{
	// Change frontend-provided strings to lowercase
	std::transform(type.begin(), type.end(), type.begin(), ::tolower);
	std::transform(category.begin(), category.end(), category.begin(), ::tolower);

	// Check that type and category are in typeCategoryItemMap_
	if (typeCategoryItemMap_.find(type) == typeCategoryItemMap_.end()
		|| typeCategoryItemMap_[type].find(category) == typeCategoryItemMap_[type].end())
	{
		// If type/category not found, return error handler
		std::cout << "ERROR: TYPE " + type + " OR CATEGORY " + category + " NOT FOUND" << std::endl;
		return new Item("NOTFOUND", "NOTFOUND", "NOTFOUND", "NOTFOUND");
	}
	
	// Check that category is in avatarItemMap_
	if (avatarItemMap_.find(category) == avatarItemMap_.end())
	{
		// If category not found, return error handler
		std::cout << "ERROR: CATEGORY " + category + " NOT FOUND" << std::endl;
		return new Item("NOTFOUND", "NOTFOUND", "NOTFOUND", "NOTFOUND");
	}

	// Lastly, check that there is an item currently equipped
	// If not, return NULL
	if (avatarItemMap_[category] == NULL)
	{
		return NULL;
	}

	// If all valid, get the icon for currently equipped item
	std::string iconName = avatarItemMap_[category]->getIconName();

	// Then use icon to search for currently equipped item
	std::vector<Item*>& itemList = typeCategoryItemMap_[type][category];
	for (unsigned int i=0; i < itemList.size(); i++)
	{
		if (itemList[i]->getIconName() == iconName)
		{
			return itemList[i];
		}
	}
	// If particular item not in category, return error handler
	std::cout << "ERROR: ITEM NOT FOUND FOR ICON " + iconName << std::endl;
	return new Item("NOTFOUND", "NOTFOUND", "NOTFOUND", "NOTFOUND");
}

DataStore::equippedItemHeap DataStore::getAllEquippedItems()
{
	equippedItemHeap equippedItems;
	
	// Iterate through categories
	avatarMap::iterator it;
	for (it = avatarItemMap_.begin(); it != avatarItemMap_.end(); ++it)
	{
		// Check that category has an item equipped
		// If yes, store the item's sprite name
		// If not, skip to next category
		if (it->second)
		{
			equippedItems.push(it->second);
		}
	}
	return equippedItems;
}

std::vector<std::string> DataStore::getTypeCategories(std::string type)
{
	// Change frontend-provided string to lowercase
	std::transform(type.begin(), type.end(), type.begin(), ::tolower);

	std::vector<std::string> categories;

	// Does the type exist?
	if (typeCategoryItemMap_.find(type) == typeCategoryItemMap_.end())
	{
		// If type not found, return error handler
		categories.push_back("ERROR: NO CATEGORIES FOUND FOR TYPE " + type);
	}
	else
	{
		// If type found, find type's categories and return them
		categoryMap& catMap = typeCategoryItemMap_[type];
		categoryMap::iterator it;
		for (it = catMap.begin(); it != catMap.end(); ++it)
		{
			categories.push_back(it->first);
			std::cout << categories[categories.size()-1] << std::endl;
		} 
	}
	return categories;
}

std::vector<Item*> DataStore::getCategoryItems(std::string type, std::string category)
{
	// Change frontend-provided strings to lowercase
	std::transform(type.begin(), type.end(), type.begin(), ::tolower);
	std::transform(category.begin(), category.end(), category.begin(), ::tolower);	

	std::vector<Item*> items;

	if (typeCategoryItemMap_.find(type) == typeCategoryItemMap_.end()
		|| typeCategoryItemMap_[type].find(category) == typeCategoryItemMap_[type].end())
	{
		// If either type or category not found, return error handler
		std::cout << "ERROR: TYPE/CATEGORY NOT FOUND FOR ITEM" << std::endl;
		items.push_back(new Item("NOTFOUND", "NOTFOUND", "NOTFOUND", "NOTFOUND"));
	}
	else
	{
		// If found, just return the item vector
		items = typeCategoryItemMap_[type][category];
	}
	return items;
}

void DataStore::printItemMap()
{
	// Print contents of map
	typeMap::iterator it;
	for (it = typeCategoryItemMap_.begin(); it != typeCategoryItemMap_.end(); ++it)
	{
		// Iterate types
		std::cout << it->first << std::endl;
		categoryMap::iterator it2;
		for (it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			// Iterate categories
			std::cout << "- " << it2->first << std::endl;
			std::vector<Item*>::iterator it3;
			for (it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
			{
				// Iterate sprite and icon names
				std::cout << "-- " << (*it3)->getSpriteName() << std::endl;
				std::cout << "-- " << (*it3)->getIconName() << std::endl;
			}
		}
	}
}

void DataStore::printAvatarMap()
{
	avatarMap::iterator it;
	for (it = avatarItemMap_.begin(); it != avatarItemMap_.end(); ++it)
	{
		std::cout << it->first << " ";
		if (it->second) std::cout << it->second->getSpriteName() << std::endl;
		else std::cout << "NULL" << std::endl;
	}
}
