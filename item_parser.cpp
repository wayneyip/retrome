#include <QDirIterator>
#include <QString>
#include <fstream>
#include <sstream>
#include "item_parser.h"
#include "util.h"

#include <iostream>

ItemParser::ItemParser()
{
	error_ = false;
}

ItemParser::~ItemParser()
{

}

bool ItemParser::parseCategories(DataStore& ds, std::string categoryFile)
{
	// Read category file
	std::ifstream ifile(categoryFile.c_str());
	if (ifile.fail())
	{
		return true;
	}

	PState state = READTYPE;
	std::string line;
	std::string currentType = "";

	while (!ifile.fail() && !error_)
	{
		// Get line and remove whitespace on either end
		getline(ifile, line);
		trim(line);

		if (state == READTYPE)
		{
			if (line[0] == '-')
			{
				state = READCATEGORY;
				line = line.substr(1, line.size()-1); // Remove bullet at front
				std::string category = trim(line);
				error_ = ds.addCategory(currentType, category);
			}
			else
			{
				std::string type = line;
				error_ = ds.addType(type);
				currentType = type;
			}
		}
		else if (state == READCATEGORY)
		{
			if (line[0] == '-')
			{
				line = line.substr(1, line.size()-1); // Remove bullet at front
				std::string category = trim(line);
				error_ = ds.addCategory(currentType, category);
			}
			else
			{
				state = READTYPE;
				std::string type = line;
				error_ = ds.addType(type);
				currentType = type;
			}			
		}
	}
	return error_;
}

bool ItemParser::parseItems(DataStore& ds, std::string imgDir, 
						std::string spriteDir, std::string iconDir)
{
	// Turn user-provided directory names into (relative) paths
	spriteDir = imgDir + "/" + spriteDir;
	iconDir = imgDir + "/" + iconDir;
	
	std::stringstream iss;
	std::string type;
	std::string category;

	// Iterate through sprite directory
	QDirIterator it(QString::fromStdString(spriteDir), 
					QDir::NoDotAndDotDot | QDir::AllEntries);
	
	while (it.hasNext())
	{
		// Go to next sprite in directory
		it.next();
		
		// For each sprite file found,
		// store the paths of the sprite and its corresponding icon
		std::string spriteName = it.fileName().toStdString(); 
		std::string spritePath = spriteDir + "/" + spriteName;
		std::string iconPath = iconDir + "/icon_" + spriteName;

		// Get the sprite's item category
		iss << spriteName;
		std::getline(iss, type, '_'); // Store type (e.g. 'body', 'clothing')
		std::getline(iss, category, '_'); // Store category (e.g. 'eyes', 'shoes')
		iss.clear(); iss.str("");

		// Create new item, add to datastore
		Item* newItem = new Item(spritePath, iconPath, type, category);
		ds.addItem(newItem);
	}
	return error_;
}