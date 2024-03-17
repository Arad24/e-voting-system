#include "Deserializer.h"

nlohmann::json getJSON(std::vector<unsigned char> buffer)
{
    std::string data(buffer.begin() + 3, buffer.end());

    return nlohmann::json::parse(data);
}

Block Deserializer::deserializeMessageBlock(std::vector<unsigned char> buffer)
{
    std::string jsonString(buffer.begin(), buffer.end());

    nlohmann::json jsonMsg = nlohmann::json::parse(jsonString);

    Block block(jsonMsg["prevHash"], jsonMsg["hash"], jsonMsg["data"], jsonMsg["index"], jsonMsg["nonce"]);

    return block;
}

Message Deserializer::deserializeMessage(const std::vector<unsigned char> buffer) {
    nlohmann::json jsonMsg = getJSON(buffer);

    Message msg(jsonMsg["id"], jsonMsg["buffer"]);

    return msg;
}

ShareKeyRequest Deserializer::deserializeShareKey(const std::vector<unsigned char> buffer) {

    nlohmann::json jsonMsg = getJSON(buffer);

    ShareKeyRequest req(jsonMsg["uid"], jsonMsg["publicKey"]);

    return req;
}

std::vector<Block> Deserializer::deserializeGetBlocksResponse(const std::vector<unsigned char> buffer)
{
    std::string jsonString(buffer.begin(), buffer.end());
    std::vector<Block> blockchain;

    try 
    {
        nlohmann::json jsonData = nlohmann::json::parse(jsonString);

        for (const auto& blockJson : jsonData)
        {
            Block block(blockJson["prevHash"].get<std::string>(),
                blockJson["blockHash"].get<std::string>(),
                blockJson["data"].get<std::string>(),
                std::stoi(blockJson["index"].get<std::string>()),
                std::stoi(blockJson["Nonce"].get<std::string>()));

            blockchain.push_back(block);
        }
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }

    return blockchain;
}

GetBlocksRequest Deserializer::deserializeGetBlocksRequest(const std::vector<unsigned char> buffer)
{
    std::string jsonString(buffer.begin(), buffer.end());

    nlohmann::json jsonData = nlohmann::json::parse(jsonString);

    GetBlocksRequest req("");

    if (jsonData.find("last_hash") != jsonData.end())
    {
        req.last_hash = jsonData["last_hash"];
    }

    return req;
}

VoteBlockData Deserializer::deserializeVoteBlockData(const std::vector<unsigned char> buffer)
{
    nlohmann::json jsonMsg = getJSON(buffer);

    VoteBlockData req(jsonMsg["sign_data"], jsonMsg["voter_id"], jsonMsg["candidate"], jsonMsg["survey_uid"]);

    return req;
}


AddVoteRequest Deserializer::deserializeAddVote(const std::vector<unsigned char> buffer)
{
    nlohmann::json jsonMsg = getJSON(buffer);

    AddVoteRequest req(jsonMsg["survey_uid"], jsonMsg["vote"]);

    return req;
}

SharePKData Deserializer::deserializeSharePKData(const std::vector<unsigned char> buffer)
{
    nlohmann::json jsonMsg = getJSON(buffer);

    SharePKData vote_request(jsonMsg["public_key"], jsonMsg["user_uid"]);

    return vote_request;
}


CountVotesRequest Deserializer::deserializeCountVotes(const std::vector<unsigned char> buffer)
{
    nlohmann::json jsonMsg = getJSON(buffer);

    CountVotesRequest req(jsonMsg["survey_uid"]);

    return req;
}

GetPeersRequest Deserializer::deserializeGetPeers(const std::vector<unsigned char> buffer)
{
    nlohmann::json jsonMsg = getJSON(buffer);
    std::vector<PeerStruct> peers;

    for (auto peer : jsonMsg["peers"])
    {
        
        std::string fixedPeer = StringUtils::removeSquareBrackets(peer.dump());
        
        if (fixedPeer[0] == '"') StringUtils::removeFirstCharacter(fixedPeer);
        if (fixedPeer[fixedPeer.length() - 1] == '"') StringUtils::removeLastCharacter(fixedPeer);

        size_t colonPos = fixedPeer.find(":");

        if (colonPos != std::string::npos)
        {
            std::string ip = std::string(fixedPeer.substr(0, colonPos));
            int port = std::stoi(fixedPeer.substr(colonPos + 1));
            PeerStruct ps(ip, port);
            peers.emplace_back(ps);
        }
    }

    GetPeersRequest req(peers);

    return req;
}