#include "item.h"

Item::Item(const std::string spriteName, const std::string iconName, 
			const std::string type, const std::string category) :
	spriteName_(spriteName),
	iconName_(iconName),
	type_(type),
	category_(category)
{

}

Item::~Item()
{

}

std::string Item::getSpriteName() const
{
	return spriteName_;
}

std::string Item::getIconName() const
{
	return iconName_;
}

std::string Item::getType() const
{
	return type_;
}

std::string Item::getCategory() const
{
	return category_;
}