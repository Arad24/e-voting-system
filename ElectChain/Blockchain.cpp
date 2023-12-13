# include "Blockchain.h"

void Blockchain::createNewBlock()
{
	Block newBlock = Block();
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
