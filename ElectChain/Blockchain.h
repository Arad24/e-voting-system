#pragma once
# include "Block.h"
# include <vector>
#include <string>

class Blockchain
{
	public:
		void addBlock(Block block);
		Block getLatestBlock();
		std::vector<Block> getBlocks();
		void createNewBlock(std::string data);

		// Getters
		Block getLatestBlock();
		std::vector<Block> getBlocks();
		
		bool validateBlock(Block currentBlock, int targetZeros, std::chrono::system_clock::time_point maxTimestamp);


	private:
		std::vector<Block> _blocks;
};