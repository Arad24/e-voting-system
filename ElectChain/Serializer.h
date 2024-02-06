#pragma once
# include "Block.h"
# include "stringUtils.h"
# include "Deserializer.h"



class Serializer
{
    public:
        static std::string serializeMessageBlock(Block block);
        static std::string serializeMessage(const Message& msg);
        static std::string serializeMessage(Response errorResponse);
        static std::string serializeMessage(SharePKData share);
        static std::string serializeMessage(VoteBlockData vote_block);

};