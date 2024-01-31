#include "stringUtils.h"

std::string StringUtils::vecToString(std::vector<unsigned char> vec)
{
    std::stringstream ss;

    // Format vector elements as string
    for (const auto& elem : vec)
    {
        ss << elem << " ";
    }

    return ss.str();
}
std::string toLower(const std::string& text)
{
    std::string lowerTxt = text;
    std::transform(lowerTxt.begin(), lowerTxt.end(), lowerTxt.begin(), [](unsigned char c) { return std::tolower(c); });

    return lowerTxt;
}

std::vector<unsigned char> strToVec(const std::string& text)
{
    std::vector<unsigned char> vec(text.begin(), text.end());
    return vec;
}

std::vector<unsigned char> strToVec(std::string str)
{
    std::vector<unsigned char> vect;
    vect.insert(vect.begin(), str.begin(), str.end());
    return vect;
}

std::string charToString(char* charArr)
{
    return std::string(charArr, charArr + strlen(charArr));
}

char* stringToChar(std::string str)
{
    char* charArray = new char[str.length() + 1];
    
    if (strcpy_s(charArray, str.length() + 1, str.c_str()) != 0) 
    {
        std::cerr << "Error copying string." << std::endl;
        delete[] charArray;
        return nullptr;
    }

    return charArray;
}
