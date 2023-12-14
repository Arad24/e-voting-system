#pragma once
# include "Block.h"
# include <vector>
# include "json.hpp"

class Deserializer
{
	public:
		static Block deserializeMessage(std::vector<unsigned char> buffer);

};