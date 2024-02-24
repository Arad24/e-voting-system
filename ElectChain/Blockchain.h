#pragma once
# include "Block.h"
#include "Serializer.h"
#include "Deserializer.h"
# include <vector>
#include <string>
#include <fstream>


# define TARGET_ZEROS 3
# define BLOCKCHAIN_FILENAME "bcCopy.csv"

class Block;

class Blockchain
{
	public:
		void addBlock(Block block);
		std::shared_ptr<Block> createNewBlock(std::string data);

		// Getters
		Block getLatestBlock();
		std::vector<Block> getBlocks();
		
		bool validateBlock(Block currentBlock);


		void saveToFile(std::string filename);
		void loadFromFile(std::string filename);
		void appendToFile(std::string filename, Block block);

		int getLastIndex();

		void display();

	private:
		std::vector<Block> _blocks;

		void deserializeBlock(const std::vector<char>& blockData, Block& block);

		
};