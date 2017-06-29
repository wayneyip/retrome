#include <QDirIterator>
#include <QString>
#include <sstream>
#include "item_parser.h"

#include <iostream>

ItemParser::ItemParser()
{

}

ItemParser::~ItemParser()
{

}

bool ItemParser::parse(DataStore& ds, std::string imgDir, 
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
	return true;
}