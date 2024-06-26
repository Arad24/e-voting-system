#include "stringUtils.h"

std::string StringUtils::vecToString(std::vector<unsigned char> vec)
{
    std::stringstream ss;

    // Format vector elements as string
    for (const auto& elem : vec)
    {
        ss << elem << "";
    }

    return ss.str();
}
std::string StringUtils::toLower(const std::string& text)
{
    std::string lowerTxt = text;
    std::transform(lowerTxt.begin(), lowerTxt.end(), lowerTxt.begin(), [](unsigned char c) { return std::tolower(c); });

    return lowerTxt;
}

std::vector<unsigned char> StringUtils::strToVec(std::string text)
{
    std::vector<unsigned char> vec(text.begin(), text.end());
    return vec;
}

std::string StringUtils::charToString(char* charArr)
{
    return std::string(charArr, charArr + strlen(charArr));
}

char* StringUtils::stringToChar(std::string str)
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


nlohmann::json StringUtils::strToJson(std::string str)
{
    return nlohmann::json::parse(str);
}

std::string StringUtils::removeSquareBrackets(std::string input) 
{
    std::string result = input;
    size_t startPos = result.find("[");
    while (startPos != std::string::npos) {
        result.erase(startPos, 1); // Erase "[" at startPos
        size_t endPos = result.find("]", startPos); // Find "]" after startPos
        if (endPos != std::string::npos) {
            result.erase(endPos, 1); // Erase "]" at endPos
        }
        startPos = result.find("[", endPos); // Find next "[" after endPos
    }
    return result;
}

void StringUtils::removeFirstCharacter(std::string& str) 
{
    if (!str.empty()) 
    {
        str.erase(0, 1); 
    }
}

void StringUtils::removeLastCharacter(std::string& str) 
{
    if (!str.empty()) 
    {
        str.pop_back();
    }
}


std::string StringUtils::base64_encode(std::string in) {
    std::string out;
    int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

std::string StringUtils::base64_decode(std::string in) {
    std::string out;
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;
    int val = 0, valb = -8;
    for (unsigned char c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}