#include "layereditem.h"

LayeredItem::LayeredItem(
		const std::string spriteName, const std::string backSpriteName, 
		const std::string iconName, 
		const std::string type, const std::string category) :
	Item(spriteName, iconName, type, category),
	backSpriteName_(backSpriteName),
	layer_(FRONT)
{
	if (category_ == "hair")
	{
		priority_ 		= 6;
		backPriority_ 	= -1; 
	}
}

LayeredItem::~LayeredItem()
{

}

std::string LayeredItem::getSpriteName() const
{
	if (layer_ == BACK)
	{
		return backSpriteName_;
	}
	return spriteName_;
}

int LayeredItem::getPriority() const
{
	if (layer_ == BACK)
	{
		return backPriority_;
	}
	return priority_;
}

void LayeredItem::toggleLayer()
{
		 if (layer_ == BACK) 	layer_ = FRONT;
	else if (layer_ == FRONT)	layer_ = BACK;
}

int LayeredItem::getLayer()
{
	return layer_;
}