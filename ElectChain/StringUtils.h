#pragma once
# include <iostream>
# include <vector>
# include <sstream>
#include <algorithm> 
#include <cctype>
#include <locale>
# include <string>
# include "json.hpp"

class StringUtils
{
    public:
        static std::string vecToString(std::vector<unsigned char> vector);
        static std::string charToString(char* charArr);
        static char* stringToChar(std::string str);
        static nlohmann::json strToJson(std::string str);
        static std::string toLower(const std::string& text);
        static std::vector<unsigned char> strToVec(std::string text);
        static std::string removeSquareBrackets(std::string input);
        static void removeFirstCharacter(std::string& str);
        static void removeLastCharacter(std::string& str);


};