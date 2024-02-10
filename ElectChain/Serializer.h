#pragma once
# include "Deserializer.h"

// Forward declarations
class Block;
struct Message;
struct ShareKeyRequest;
struct Response;
struct SharePKData;
struct VoteBlockData;

class Serializer
{
    public:
        static std::string serializeMessageBlock(Block block);
        static std::string serializeMessage(Message msg);
        static std::string serializeMessage(Response Response, std::string code);
        static std::string serializeMessage(SharePKData share);
        static std::string serializeMessage(VoteBlockData vote_block);
};