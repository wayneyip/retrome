#include "item.h"

Item::Item(const std::string spriteName, const std::string iconName, 
			const std::string type, const std::string category) :
	spriteName_(spriteName),
	iconName_(iconName),
	type_(type),
	category_(category)
{
	if (type_ == "body")
	{
		if (category_ == "eyes") priority_ = 0;
		else if (category_ == "nose") priority_ = 1;
		else if (category_ == "mouth") priority_ = 2;
		else if (category_ == "eyebrows") priority_ = 3;
		else if (category_ == "hair") priority_ = 4;
		else if (category_ == "ears") priority_ = 5;
	}
	else if (type_ == "clothing")
	{
		if (category_ == "bottom") priority_ = 6;
		else if (category_ == "top") priority_ = 7;
		else if (category_ == "shoes") priority_ = 8;		
	}
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

int Item::getPriority() const
{
	return priority_;
}