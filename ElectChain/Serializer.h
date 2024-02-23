#pragma once
# include "Deserializer.h"

// Forward declarations
class Block;
struct PeerStruct;
class Peer;
struct Response;
struct Message;
struct ShareKeyRequest;
struct SharePKData;
struct VoteBlockData;
struct AddVoteRequest;
struct CountVotesRequest;
struct GetPeersRequest;
struct CountVotesResponse;

class Serializer
{
    public:
        static std::string serializeMessageBlock(Block block);
        static std::string serializeMessage(Message msg);
        static std::string serializeMessage(Response Response, std::string code);
        static std::string serializeMessage(SharePKData share);
        static std::string serializeMessage(VoteBlockData vote_block);
        static std::string serializeMessage(CountVotesResponse res);
};