#include "Deserializer.h"

nlohmann::json getJSON(std::vector<unsigned char> buffer)
{
    std::string data(buffer.begin(), buffer.end());

    return nlohmann::json::parse(data);
}

Block Deserializer::deserializeMessageBlock(std::vector<unsigned char> buffer)
{
    nlohmann::json jsonMsg = getJSON(buffer);

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

    AddVoteRequest req(jsonMsg["vote"], jsonMsg["survey_uid"]);

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

    for (const auto& peer : jsonMsg["peers"]) 
    {
        std::string strPeer = peer.dump();
        std::string ip = strPeer.substr(0, strPeer.find(":"));
        int port = stoi(strPeer.substr(strPeer.find(":") + 1, strPeer.length()));
        PeerStruct newPeer(ip, port);
        peers.push_back(newPeer);
    }

    GetPeersRequest req(peers);

    return req;
}