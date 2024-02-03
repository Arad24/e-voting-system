#pragma once
# include "Block.h"
# include <vector>
# include "json.hpp"
#include "Peer.h"
#include "structs.h"

struct ShareKeyRequest {
    int userId;
    std::vector<unsigned char> publicKey;
    std::time_t timestamp;

    ShareKeyRequest(int id, std::vector<unsigned char> key, std::time_t time) : userId(id), publicKey(key), timestamp(time) {}
};

struct ErrorResponse
{
    std::string message;
};

struct SharePK {
    std::string public_key;
    std::string voter_id;

    SharePK(std::string public_key1, std::string voter_id1) : public_key(public_key1), voter_id(voter_id1) {}
};

struct VoteBlock {
    std::string sign_data;
    std::string voter_id;
    std::string candidate;
    std::string survey_id;
    VoteBlock(std::string sign_data1, std::string voter_id1, std::string candidate1, std::string survey_id1) :sign_data(sign_data1), voter_id(voter_id1), candidate(candidate1), survey_id(survey_id1) {}
};


class Deserializer
{
public:
    static Block deserializeMessageBlock(std::vector<unsigned char> buffer);
    static Message deserializeMessage(const std::vector<unsigned char> buffer);
    static ShareKeyRequest deserializeShareKey(const std::vector<unsigned char> buffer);
    static std::vector<Block> deserializeMessageBlocks(const std::vector<unsigned char> buffer);
    static SharePK deserializeSharePK(const std::vector<unsigned char> buffer);
    static VoteBlock deserializeVoteBlock(const std::vector<unsigned char> buffer);

};