#pragma once
# include "Block.h"
# include <vector>
# include "json.hpp"
#include "Peer.h"
#include "structs.h"

class Deserializer
{
	public:
		static Block deserializeMessageBlock(std::vector<unsigned char> buffer);
		static Message deserializeMessage(const std::vector<unsigned char> buffer);
		static ShareKeyRequest deserializeShareKey(const std::vector<unsigned char> buffer);
		static std::vector<Block> deserializeMessageBlocks(const std::vector<unsigned char> buffer);

};