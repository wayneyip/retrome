#ifndef LAYEREDITEM_H
#define LAYEREDITEM_H
#include "item.h"

class LayeredItem : public Item
{
public:
	enum Layer { FRONT, BACK };
	
	LayeredItem(const std::string spriteName, const std::string backSpriteName,
		const std::string iconName, 
		const std::string type, const std::string category);
	~LayeredItem();

	std::string getSpriteName() const;
	int getPriority() const;

	void toggleLayer();
	int getLayer();

private:
	std::string backSpriteName_;
	int backPriority_;
	int layer_;
};

#endif