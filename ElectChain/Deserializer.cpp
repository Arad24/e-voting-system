#include "Deserializer.h"

nlohmann::json getJSON(std::vector<unsigned char> buffer)
{
    std::string data(buffer.begin(), buffer.end());

    return nlohmann::json::parse(data);
}

Block Deserializer::deserializeMessage(std::vector<unsigned char> buffer)
{
    nlohmann::json jsonMsg = getJSON(buffer);

    Block block(jsonMsg["prevHash"], jsonMsg["hash"], jsonMsg["data"], jsonMsg["timestamp"]);

    return block;
}

Message Deserializer::deserializeMessage(const std::vector<unsigned char>& buffer) {
    nlohmann::json jsonMsg = getJSON(buffer);

    Message msg(jsonMsg["userID"], jsonMsg["content"]);

    return msg;
}

