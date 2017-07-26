#ifndef COLOR_H
#define COLOR_H

#include <vector>
#include <string>

class Color
{
public:
	typedef std::vector<unsigned int> shade;

	Color(const std::vector<shade> shades,
		const std::string category);
	~Color();
	shade getMainColor() const;
	std::vector<shade> getShades() const;
	std::string getCategory() const;

private:
	shade mainColor_;
	std::vector<shade> shades_;
	std::string category_;
};

#endif