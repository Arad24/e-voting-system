#include "Deserializer.h"

nlohmann::json getJSON(std::vector<unsigned char> buffer)
{
    std::string data(buffer.begin(), buffer.end());

    return nlohmann::json::parse(data);
}

Block Deserializer::deserializeMessageBlock(std::vector<unsigned char> buffer)
{
    nlohmann::json jsonMsg = getJSON(buffer);

    Block block(jsonMsg["prevHash"], jsonMsg["hash"], jsonMsg["data"], jsonMsg["timestamp"], jsonMsg["nonce"]);

    return block;
}

Message Deserializer::deserializeMessage(const std::vector<unsigned char> buffer) {
    nlohmann::json jsonMsg = getJSON(buffer);

    Message msg(jsonMsg["id"], jsonMsg["buffer"]);

    return msg;
}

ShareKeyRequest Deserializer::deserializeShareKey(const std::vector<unsigned char> buffer) {

    nlohmann::json jsonMsg = getJSON(buffer);

    ShareKeyRequest req(jsonMsg["uid"], jsonMsg["publicKey"], jsonMsg["timestamp"]);

    return req;
}

std::vector<Block> Deserializer::deserializeGetBlocks(const std::vector<unsigned char> buffer)
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

VoteBlockData Deserializer::deserializeVoteBlockData(const std::vector<unsigned char> buffer)
{
    nlohmann::json jsonMsg = getJSON(buffer);

    VoteBlockData req(jsonMsg["sign_data"], jsonMsg["voter_id"], jsonMsg["candidate"], jsonMsg["survey_id"]);

    return req;
}

SharePKData Deserializer::deserializeSharePKData(const std::vector<unsigned char> buffer)
{
    nlohmann::json jsonMsg = getJSON(buffer);

    SharePKData vote_request(jsonMsg["public_key"], jsonMsg["voter_id"]);

    return vote_request;
}
