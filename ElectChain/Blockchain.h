#pragma once
# include "Block.h"
# include <vector>

class Blockchain
{
	public:
		void createNewBlock();

	private:
		std::vector<Block> _blocks;
};