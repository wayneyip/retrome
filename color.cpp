#include "color.h"

Color::Color(const std::vector<std::vector<unsigned char> > shades,
			const std::string category) :
	shades_(shades),
	category_(category)
{
	mainColor_ = shades_[0];
}

Color::~Color()
{

}

std::vector<unsigned char> Color::getMainColor() const
{
	return mainColor_;
}

std::vector<std::vector<unsigned char> > Color::getShades() const
{
	return shades_;
}

std::string Color::getCategory() const
{
	return category_;
}
