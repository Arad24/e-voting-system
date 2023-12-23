# include "Blockchain.h"


void Blockchain::createNewBlock(std::string data)
{
	Block newBlock = Block(data);
}


void Blockchain::addBlock(Block block)
{
	if (validateBlock(block))
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

bool Blockchain::validateBlock(Block block) 
{
    if (!_blocks.empty()) 
    {
        Block previousBlock = _blocks.back();

        if (block.getPrevHash() != previousBlock.getHash())
        {
            std::cout << "Previous hash does not match." << std::endl;
            return false;
        }
    }
    
    if (!BlockchainUtils::isValidHash(block.getHash())) {
        std::cout << "Invalid proof-of-work in the block." << std::endl;
        return false;
    }

    std::chrono::system_clock::time_point blockTimestamp = std::chrono::system_clock::from_time_t(std::stoi(block.getTimeStamp()));
    std::chrono::system_clock::time_point currentTimestamp = std::chrono::system_clock::now(); 
    
    if (blockTimestamp > currentTimestamp)
    {
        std::cout << "Block timestamp is too far in the future." << std::endl;
        return false;
    }
    return true;
}
