#pragma once
# include <iostream>
# include <vector>
# include <sstream>
# include <algorithm>
# include <cctype>
# include <string>

class StringUtils
{
    public:
        static std::string vecToString(std::vector<unsigned char> vector);
        static std::string charToString(char* charArr);
        static char* stringToChar(std::string str);
        std::string toLower(const std::string& text);
        std::vector<unsigned char> strToVec(const std::string& text);

};