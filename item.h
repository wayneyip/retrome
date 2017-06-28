#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item
{
public:
	Item(const std::string spriteName, const std::string iconName, 
		const std::string type, const std::string category);
	virtual ~Item();

	std::string getSpriteName() const;
	std::string getIconName() const;
	std::string getType() const;
	std::string getCategory() const;

private:
	std::string spriteName_;
	std::string iconName_;
	std::string type_;
	std::string category_;	
};

#endif