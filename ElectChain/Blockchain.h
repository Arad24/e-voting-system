#pragma once
# include "Block.h"
# include <vector>
#include <string>

# define TARGET_ZEROS 2

class BlockchainUtils;
class Block;

class Blockchain
{
	public:
		void addBlock(Block block);
		void createNewBlock(std::string data);

		// Getters
		Block getLatestBlock();
		std::vector<Block> getBlocks();
		
		bool validateBlock(Block currentBlock);


	private:
		std::vector<Block> _blocks;
};