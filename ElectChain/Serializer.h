#pragma once
# include "Block.h"
# include "stringUtils.h"
# include "json.hpp"
#include "Peer.h"
#include "structs.h"

class Serializer
{
	public:
		static std::string serializeMessageBlock(Block block);
		static std::string serializeMessage(const Message& msg);
		static std::string serializeErrorResponse(struct ErrorResponse errorResponse);


	private:
		
};