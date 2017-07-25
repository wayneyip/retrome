#ifndef COLOR_H
#define COLOR_H

#include <vector>
#include <string>

class Color
{
public:
	Color(const std::vector<std::vector<unsigned char> > shades,
		const std::string category);
	~Color();
	std::vector<unsigned char> getMainColor() const;
	std::vector<std::vector<unsigned char> > getShades() const;
	std::string getCategory() const;

private:
	std::vector<unsigned char> mainColor_;
	std::vector<std::vector<unsigned char> > shades_;
	std::string category_;
};

#endif