#pragma once
# include "Block.h"
# include <vector>
#include <string>

class Blockchain
{
	public:
		void createNewBlock();
		bool validateBlock(Block currentBlock, int targetZeros,  std::chrono::system_clock::time_point maxTimestamp);

	private:
		std::vector<Block> _blocks;
};