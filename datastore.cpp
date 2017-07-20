#include "datastore.h"
#include <algorithm>
#include <cstdlib>
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
		categoryTypeMap_.insert(std::make_pair(category, type));
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
			categoryTypeMap_.insert(std::make_pair(category, type));
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
	avatarItemMap_[item->getCategory()] = item;
}

void DataStore::removeItem(Item* item)
{
	avatarItemMap_[item->getCategory()] = NULL;
}

Item* DataStore::findEquippedItem(std::string type, std::string category)
{
	// Change frontend-provided strings to lowercase
	std::transform(type.begin(), type.end(), type.begin(), ::tolower);
	std::transform(category.begin(), category.end(), category.begin(), ::tolower);

	// Check that there is an item currently equipped
	// If not, return NULL
	if (avatarItemMap_[category] == NULL)
	{
		return NULL;
	}

	// If item is equipped, get the icon for currently equipped item
	std::string iconName = avatarItemMap_[category]->getIconName();

	// Then search through the item's category for that item,
	// using the icon's name to match
	std::vector<Item*>& itemList = typeCategoryItemMap_[type][category];
	for (unsigned int i=0; i < itemList.size(); i++)
	{
		if (itemList[i]->getIconName() == iconName)
		{
			return itemList[i];
		}
	}
	return NULL;
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

	// Access the type's categories from the overarching map
	categoryMap& catMap = typeCategoryItemMap_[type];
	
	// Then make a container to hold the categories
	std::vector<std::string> categories;
	
	// Loop in the category strings, from map to container
	categoryMap::iterator it;
	for (it = catMap.begin(); it != catMap.end(); ++it)
	{
		categories.push_back(it->first);
	} 

	return categories;
}

std::vector<Item*> DataStore::getCategoryItems(std::string type, std::string category)
{
	// Change frontend-provided strings to lowercase
	std::transform(type.begin(), type.end(), type.begin(), ::tolower);
	std::transform(category.begin(), category.end(), category.begin(), ::tolower);	

	// Just return the item vector itself
	return typeCategoryItemMap_[type][category];
}

std::string DataStore::getCategoryType(std::string category)
{
	return categoryTypeMap_[category];
}

void DataStore::selectRandomItems()
{
	srand(time(NULL));
	avatarMap::iterator it;
	
	// For each category...
	for (it = avatarItemMap_.begin(); it != avatarItemMap_.end(); ++it)
	{
		// Get all items under the category
		std::string category = it->first;
		std::string type = getCategoryType(category);
		std::vector<Item*> categoryItems = getCategoryItems(type, category);
		
		// Get the index of a (random) item, and equip it
		int index = rand() % categoryItems.size();
		selectItem(categoryItems[index]); 
	}
}


//***DEBUG FUNCTIONS***//

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
