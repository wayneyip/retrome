#include "datastore.h"
#include "util.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

DataStore::DataStore()
{

}

DataStore::~DataStore()
{

}

bool DataStore::addType(std::string type)
{
	// Add type to master map,
	// with an empty map of categories to items
	std::map<std::string, std::vector<Item*> > categoryItemMap;
	typeCategoryItemMap_.insert(std::make_pair(type, categoryItemMap));

	// Repeat for color map
	std::map<std::string, std::vector<Color*> > categoryColorMap;
	typeCategoryColorMap_.insert(std::make_pair(type, categoryColorMap));

	// No errors found
	return false;
}

bool DataStore::addCategory(std::string type, std::string category)
{
	// Error check: ensure that given type exists
	if (typeCategoryItemMap_.find(type) == typeCategoryItemMap_.end())
	{
		return true;
	}

	// Add category to master map under its type,
	// with an empty list of items
	std::vector<Item*> itemList;
	typeCategoryItemMap_[type].insert(std::make_pair(category, itemList));
	
	// Repeat for color map
	std::vector<Color*> colorList;
	typeCategoryColorMap_[type].insert(std::make_pair(category, colorList));

	// Add category to avatar map,
	// and second map that traces back to its type
	Item* NULLITEM = NULL;
	avatarItemMap_.insert(std::make_pair(category, NULLITEM));
	categoryTypeMap_.insert(std::make_pair(category, type));

	// No errors found
	return false;
}

bool DataStore::addColor(Color* color)
{
	std::string category = color->getCategory();

	// Error check: ensure that type and category exist
	if (categoryTypeMap_.find(category) == categoryTypeMap_.end())
	{
		return true;
	}
	std::string type = categoryTypeMap_[category];
	if (typeCategoryColorMap_.find(type) 
			== typeCategoryColorMap_.end())
	{
		return true;
	}
	if (typeCategoryColorMap_[type].find(category)
			== typeCategoryColorMap_[type].end())
	{
		return true;
	}

	// Add color to map under its category
	typeCategoryColorMap_[type][category].push_back(color);

	// No errors found
	return false;
}

bool DataStore::addItem(Item* item)
{
	std::string type = item->getType();
	std::string category = item->getCategory();

	// Error check: ensure that given type and category exist
	if (typeCategoryItemMap_.find(type) == typeCategoryItemMap_.end())
	{
		return true;
	}
	if (typeCategoryItemMap_[type].find(category) 
				== typeCategoryItemMap_[type].end())
	{
		return true;
	}

	// Add item to master map under its type and category
	typeCategoryItemMap_[type][category].push_back(item);

	// No errors found
	return false;
}

std::vector<std::string> DataStore::getTypeCategories(std::string type)
{
	// Change frontend-provided string to lowercase
	convToLower(type);

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

std::string DataStore::getCategoryType(std::string category)
{
	return categoryTypeMap_[category];
}

std::vector<Item*> DataStore::getCategoryItems(std::string category)
{
	// Change frontend-provided strings to lowercase
	convToLower(category);
	std::string type = getCategoryType(category);

	// Just return the item vector itself
	return typeCategoryItemMap_[type][category];
}

void DataStore::selectItem(Item* item)
{
	avatarItemMap_[item->getCategory()] = item;
}

Item* DataStore::findEquippedItem(std::string category)
{
	// Change frontend-provided strings to lowercase
	convToLower(category);
	std::string type = getCategoryType(category);

	return avatarItemMap_[category];
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

void DataStore::selectRandomItems()
{
	srand(time(NULL));
	avatarMap::iterator it;
	
	// For each category...
	for (it = avatarItemMap_.begin(); it != avatarItemMap_.end(); ++it)
	{
		// Get all items under the category
		std::string category = it->first;
		std::vector<Item*> categoryItems = getCategoryItems(category);
		
		// Get the index of a (random) item, and equip it
		int index = rand() % categoryItems.size();
		selectItem(categoryItems[index]);
	}
}

std::vector<Color*> DataStore::getCategoryColors(std::string category)
{
	// Change frontend-provided strings to lowercase
	convToLower(category);
	std::string type = getCategoryType(category);

	return typeCategoryColorMap_[type][category];
}

void DataStore::changeColor(Color* color)
{
	avatarColorMap_[color->getCategory()] = color;
}

Color* DataStore::findSelectedColor(std::string category)
{
	// Change frontend-provided strings to lowercase
	convToLower(category);
	std::string type = getCategoryType(category);

	if (category == "face" || category == "ears")
	{
		return avatarColorMap_["body"];
	}

	return avatarColorMap_[category];
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
			std::vector<Color*>& colors = typeCategoryColorMap_[it->first][it2->first];
			for (unsigned int i=0; i < colors.size(); i++)
			{
				std::cout << "~~ ";
				std::vector<Color::shade> shades = colors[i]->getShades();
				for (unsigned int j=0; j < shades.size(); j++)
				{
					for (unsigned int k=0; k < shades[j].size(); k++)
					{
						std::cout << (int)shades[j][k] << " ";
					}
					std::cout << "| ";
				}
				std::cout << std::endl;
			}
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

void DataStore::PRINT(std::string statement)
{
	std::cout << statement << std::endl;
}