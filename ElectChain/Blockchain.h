#pragma once
# include "Block.h"
# include <vector>

class Blockchain
{
	public:
		void createNewBlock();
		void Blockchain::addBlock(Block block)
		Block Blockchain::getLatestBlock()
		std::vector<Block> Blockchain::getBlocks()

	private:
		std::vector<Block> _blocks;
};