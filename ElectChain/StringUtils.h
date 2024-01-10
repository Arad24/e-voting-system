#pragma once
# include <iostream>
# include <vector>
# include <sstream>

class StringUtils
{
    public:
        static std::string vecToString(std::vector<unsigned char> vector);
        static std::string charToString(char* charArr);
        static char* stringToChar(std::string str);
};