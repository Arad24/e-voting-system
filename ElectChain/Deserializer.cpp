#include "Deserializer.h"

nlohmann::json getJSON(std::vector<unsigned char> buffer)
{
    std::string data(buffer.begin(), buffer.end());

    return nlohmann::json::parse(data);
}

Block Deserializer::deserializeMessageBlock(std::vector<unsigned char> buffer)
{
    nlohmann::json jsonMsg = getJSON(buffer);

    Block block(jsonMsg["prevHash"], jsonMsg["hash"], jsonMsg["data"], jsonMsg["timestamp"]);

    return block;
}

Message Deserializer::deserializeMessage(const std::vector<unsigned char> buffer) {
    nlohmann::json jsonMsg = getJSON(buffer);

    Message msg(jsonMsg["id"], jsonMsg["buffer"], jsonMsg["timestamp"]);

    return msg;
}

ShareKeyRequest Deserializer::deserializeShareKey(const std::vector<unsigned char> buffer) {

    nlohmann::json jsonMsg = getJSON(buffer);

    ShareKeyRequest req(jsonMsg["userId"], jsonMsg["publicKey"], jsonMsg["timestamp"]);

    return req;
}

std::vector<Block> Deserializer::deserializeMessageBlocks(const std::vector<unsigned char> buffer)
{
    std::string jsonString(buffer.begin(), buffer.end());

    nlohmann::json jsonMsgArray = nlohmann::json::parse(jsonString);

    if (!jsonMsgArray.is_array()) {
        return std::vector<Block>();
    }

    std::vector<Block> deserializedBlocks;
    for (const auto& jsonMsg : jsonMsgArray) {
        deserializedBlocks.push_back(Block(jsonMsg["prevHash"], jsonMsg["hash"], jsonMsg["data"], jsonMsg["timestamp"]));
    }

    return deserializedBlocks;
}