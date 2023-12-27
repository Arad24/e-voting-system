#pragma once
# include "Block.h"
# include <vector>
# include "json.hpp"
#include "Peer.h"
class Deserializer
{
	public:
		static Block deserializeMessage(std::vector<unsigned char> buffer);
		static Message deserializeMessage(const std::vector<unsigned char>& buffer);

};