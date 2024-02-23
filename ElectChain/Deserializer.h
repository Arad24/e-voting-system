#pragma once
# include "Block.h"
# include "json.hpp"
# include "Peer.h"
# include <vector>

class Block;
struct PeerStruct;
class Peer;
class BlockRequestHandler;

struct Response
{
    std::string message;
};

struct Message
{
    std::string id;
    std::vector<unsigned char> buffer;

    Message(std::string id, std::vector<unsigned char> msg) : id(id), buffer(msg) {}
} typedef Message;

struct ShareKeyRequest
{
    std::string user_uid;
    std::vector<unsigned char> publicKey;

    ShareKeyRequest(std::string id, std::vector<unsigned char> key) : user_uid(id), publicKey(key) {}
} typedef ShareKeyRequest;

struct SharePKData
{
    std::string public_key;
    std::string user_uid;

    SharePKData(std::string public_key1, std::string user_uid1) : public_key(public_key1), user_uid(user_uid1) {}
} typedef SharePKData;

struct VoteBlockData
{
    std::string sign_data;
    std::string user_uid;
    std::string vote;
    std::string survey_uid;
    VoteBlockData(std::string sign_data1, std::string user_id1, std::string vote1, std::string survey_id1) :sign_data(sign_data1), user_uid(user_id1), vote(vote1), survey_uid(survey_id1) {}
} typedef VoteBlockData;

struct CountVotesRequest
{
    std::string survey_uid;
    CountVotesRequest(std::string survey_uid1) : survey_uid(survey_uid1) {}
} typedef CountVotesRequest;

struct GetPeersRequest
{
    std::vector<PeerStruct> peers;
    GetPeersRequest(std::vector<PeerStruct> peers1) : peers(peers1) {}
} typedef GetPeersRequest;

struct AddVoteRequest
{
    std::string vote;
    std::string survey_uid;
    AddVoteRequest(std::string survey_uid1, std::string vote1) : survey_uid(survey_uid1), vote(vote1) {}

} typedef AddVoteRequest;

struct CountVotesResponse
{
    std::map<std::string, int> votes;
    CountVotesResponse(std::map<std::string, int> votes1) : votes(votes1) {}
} typedef CountVotesResponse;


# define ADD_BLOCK_CODE "100"
# define SHARE_KEY_CODE "101"
# define GET_PEERS_LIST "102"
# define GET_BLOCKCHAIN "103"
# define LOGIN_CODE "104"
# define PEERS_LIST_SUCCEEDED_CODE "202"
# define LOGIN_SUCCEEDED_CODE "204"
# define ADD_VOTE_CODE "300"
# define COUNT_VOTES_CODE "301"
# define LOGIN_FAILED_CODE "404"
# define DONT_SEND_CODE "0"
# define ERROR_ADD_VOTE "400"
# define SUCCESS_ADD_VOTE "500"
# define ERROR_COUNT_VOTES "401"
# define SUCCESS_COUNT_VOTES "501"

class Deserializer
{
    public:
        static Block deserializeMessageBlock(std::vector<unsigned char> buffer);
        static Message deserializeMessage(const std::vector<unsigned char> buffer);
        static ShareKeyRequest deserializeShareKey(const std::vector<unsigned char> buffer);
        static std::vector<Block> deserializeGetBlocks(const std::vector<unsigned char> buffer);
        static SharePKData deserializeSharePKData(const std::vector<unsigned char> buffer);
        static VoteBlockData deserializeVoteBlockData(const std::vector<unsigned char> VoteBlockData);
        static AddVoteRequest deserializeAddVote(const std::vector<unsigned char> buffer);
        static CountVotesRequest deserializeCountVotes(const std::vector<unsigned char> buffer);
        static GetPeersRequest deserializeGetPeers(const std::vector<unsigned char> buffer);
};