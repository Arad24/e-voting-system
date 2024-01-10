#pragma once
# include "Block.h"
# include <vector>
#include <string>
#include <fstream>
#include "Serializer.h"
#include "Deserializer.h"

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

		void saveToFile(const std::string& filename);
		void loadFromFile(const std::string& filename);
	private:
		std::vector<Block> _blocks;
};