#include <QDirIterator>
#include <QString>
#include <fstream>
#include <sstream>
#include <vector>

#include "item_parser.h"
#include "util.h"
#include "color.h"

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

	std::string line;
	std::string currentType = "";
	std::string currentCategory = "";

	while (!ifile.fail() && !error_)
	{
		// Get line and remove whitespace on either end
		std::getline(ifile, line);
		trim(line);

		if (line[0] == '-') // category
		{
			removeBullet(line);
			std::string category = trim(line);
			error_ = ds.addCategory(currentType, category);
			currentCategory = category;
		}
		else if (line[0] == '~') // color
		{
			removeBullet(line);
			error_ = parseColors(ds, line, currentCategory);
		}
		else // type
		{
			std::string type = line;
			error_ = ds.addType(type);
			currentType = type;
		}
	}
	// ds.printItemMap();
	return error_;
}

bool ItemParser::parseColors(DataStore& ds, std::string colors, std::string category)
{
	std::stringstream ss(colors);
	std::string dump; // dummy string to remove opening bracket '[';
	std::string colorString;
	std::stringstream colorStream;

	std::vector<std::vector<unsigned char> > colorList;

	while (std::getline(ss, dump, '['))
	{
		// Get individual RGB values between brackets
		std::getline(ss, colorString, ']');
		colorStream << colorString;
		int r, g, b;
		colorStream >> r >> g >> b;
		std::cout << r << " " << g << " " << b << std::endl;

		// Store RGB values into a vector
		std::vector<unsigned char> rgb;
		rgb.push_back((unsigned char) r);
		rgb.push_back((unsigned char) g);
		rgb.push_back((unsigned char) b);

		// This vector becomes one shade of the overall Color
		colorList.push_back(rgb);

		colorStream.clear(); colorStream.str("");
	}
	
	Color* newColor = new Color(colorList, category);
	return ds.addColor(newColor);
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