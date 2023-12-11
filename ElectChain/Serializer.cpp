#include "Serializer.h"

std::string Serializer::serializeMessage(Block block)
{
    std::vector<unsigned char> message;
    nlohmann::json jsMsg = {};

    jsMsg["prevHash"] = block.getPrevHash();
    jsMsg["hash"] = block.getHash();
    jsMsg["data"] = block.getData();
    jsMsg["timestamp"] = block.getTimeStamp();

    for (char c : nlohmann::to_string(jsMsg))
    {
        message.push_back(c);
    }

    return StringUtils::vecToString(message);
}
