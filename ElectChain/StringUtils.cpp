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
