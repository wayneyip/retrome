#include <algorithm>
<<<<<<< HEAD
#include <functional>
#include <cctype>
#include <locale>
=======
#include <cctype>
>>>>>>> 6336fb2baad7f6969e303e52be0ae1008e6fab07
#include "util.h"

std::string &convToLower(std::string &src)
{
    std::transform(src.begin(), src.end(), src.begin(), ::tolower);
    return src;
}

std::string &removeBullet(std::string &line)
{
	line = line.substr(1, line.size()-1);
	return line;
}

// trim from start
std::string &ltrim(std::string &s) {
    s.erase(s.begin(), 
	    std::find_if(s.begin(), 
			 s.end(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string &rtrim(std::string &s) {
    s.erase(
	    std::find_if(s.rbegin(), 
			 s.rend(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))).base(), 
	    s.end());
    return s;
}

// trim from both ends
std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
<<<<<<< HEAD
}
=======
}
>>>>>>> 6336fb2baad7f6969e303e52be0ae1008e6fab07
