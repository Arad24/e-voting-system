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
		Blockchain();

		void addBlock(Block block);
		std::shared_ptr<Block> createNewBlock(std::string data);
		void addGenesisBlock();

		// Getters
		Block getLatestBlock();
		std::vector<Block> getBlocks();
		
		bool validateBlock(Block currentBlock);


		void saveToFile();
		void loadFromFile();
		void appendToFile(Block block);

		int getLastIndex();

		int getChainLength();
		void updateChainLength();
		void updateChain(std::vector<Block> newChain);
		std::vector<Block> isLongestChain(std::string hash, std::vector<Block> newChain);


		void display();

	private:
		std::vector<Block> _blocks;
		int _chainLength;
		
};