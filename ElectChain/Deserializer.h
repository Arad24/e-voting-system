#pragma once
# include "Block.h"
# include "json.hpp"
# include <vector>

class Block;

struct Message
{
    std::string id;
    std::vector<unsigned char> buffer;

    Message(std::string id, std::vector<unsigned char> msg) : id(id), buffer(msg) {}
} typedef Message;

struct ShareKeyRequest
{
    std::string uid;
    std::vector<unsigned char> publicKey;
    std::time_t timestamp;

    ShareKeyRequest(std::string id, std::vector<unsigned char> key, std::time_t time) : uid(id), publicKey(key), timestamp(time) {}
} typedef ShareKeyRequest;

struct Response
{
    std::string message;
};

struct SharePKData
{
    std::string public_key;
    std::string voter_id;

    SharePKData(std::string public_key1, std::string voter_id1) : public_key(public_key1), voter_id(voter_id1) {}
} typedef SharePKData;

struct VoteBlockData
{
    std::string sign_data;
    std::string voter_id;
    std::string candidate;
    std::string survey_id;
    VoteBlockData(std::string sign_data1, std::string voter_id1, std::string candidate1, std::string survey_id1) :sign_data(sign_data1), voter_id(voter_id1), candidate(candidate1), survey_id(survey_id1) {}
} typedef VoteBlockData;


# define ADD_BLOCK_CODE "100"
# define SHARE_KEY_CODE "101"
# define GET_PEERS_LIST "102"
# define GET_BLOCKCHAIN "103"
# define LOGIN_CODE "104"
# define LOGIN_SUCCEEDED_CODE "204"
# define ADD_VOTE_CODE "300"
# define COUNT_VOTES_CODE "301"
# define LOGIN_FAILED_CODE "404"
# define DONT_SEND_CODE "0"

class Deserializer
{
    public:
        static Block deserializeMessageBlock(std::vector<unsigned char> buffer);
        static Message deserializeMessage(const std::vector<unsigned char> buffer);
        static ShareKeyRequest deserializeShareKey(const std::vector<unsigned char> buffer);
        static std::vector<Block> deserializeGetBlocks(const std::vector<unsigned char> buffer);
        static SharePKData deserializeSharePKData(const std::vector<unsigned char> buffer);
        static VoteBlockData deserializeVoteBlockData(const std::vector<unsigned char> VoteBlockData);
};