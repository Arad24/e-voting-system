# include "Blockchain.h"


void Blockchain::createNewBlock(std::string data)
{
	Block newBlock = Block(data);
}

void Blockchain::addBlock(Block block)
{
	if (block.isValid())
	{
		_blocks.push_back(block);
	}
}

Block Blockchain::getLatestBlock()
{
	return _blocks.back();
}

std::vector<Block> Blockchain::getBlocks()
{
	return this->_blocks;
}


