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
    
    if (!BlockchainUtils::isValidHash(block.getHash())) 
    {
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

    /* Add check of the block signature */
    return true;
}

//save blockchain to binary file
void Blockchain::saveToFile(const std::string& filename)
{
    std::ofstream file(filename, std::ios::binary | std::ios::out);
    if (!file.is_open()) 
    {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    // Serialize each block and write it to the file
    for (const Block& block : _blocks) 
    {
        std::string serializedBlock = Serializer::serializeMessageBlock(block);
        file.write(serializedBlock.data(), serializedBlock.size());
    }

    file.close();
}

//load blockchain from binary file
void Blockchain::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::in);
    if (!file.is_open()) 
    {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return;
    }

    _blocks.clear();

    std::vector<unsigned char> serializedData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    std::vector<Block> deserializedBlocks = Deserializer::deserializeMessageBlocks(serializedData);
    _blocks.insert(_blocks.end(), deserializedBlocks.begin(), deserializedBlocks.end());
}

