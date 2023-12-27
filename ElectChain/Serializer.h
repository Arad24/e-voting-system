#pragma once
# include "Block.h"
# include "stringUtils.h"
# include "json.hpp"
#include "Peer.h"
class Serializer
{
	public:
		static std::string serializeMessage(Block block);
		static std::string serializeMessage(const Message& msg);

	private:
		
};