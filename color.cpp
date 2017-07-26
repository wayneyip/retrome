#include "color.h"

Color::Color(const std::vector<shade> shades,
			const std::string category) :
	shades_(shades),
	category_(category)
{
	mainColor_ = shades_[0];
}

Color::~Color()
{

}

Color::shade Color::getMainColor() const
{
	return mainColor_;
}

std::vector<Color::shade> Color::getShades() const
{
	return shades_;
}

std::string Color::getCategory() const
{
	return category_;
}
