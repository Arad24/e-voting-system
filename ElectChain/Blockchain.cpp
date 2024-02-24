# include "Blockchain.h"

std::shared_ptr<Block> Blockchain::createNewBlock(std::string data)
{
	std::shared_ptr<Block> newBlock = std::make_shared<Block>(data);
    return newBlock;
}


void Blockchain::addBlock(Block block)
{
	if (validateBlock(block))
	{
		_blocks.push_back(block);
        appendToFile(BLOCKCHAIN_FILENAME, block);
	}
}

int Blockchain::getLastIndex()
{
    int lastIndex = (_blocks.empty()) ? (0) : (getLatestBlock().getIndex());
    return lastIndex;
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

    if (block.getIndex() - 1 != getLastIndex()) return false;

    return true;
}

// Utility function to display block_blocks contents
void Blockchain::display() 
{
    for (auto& block : _blocks) 
    {
        std::cout << "Block:" << std::endl;
        std::cout << "  Previous Hash: " << block.getPrevHash() << std::endl;
        std::cout << "  Hash: " << block.getHash() << std::endl;
        std::cout << "  Data: " << block.getData() << std::endl;
        std::cout << "  index: " << block.getIndex() << std::endl;
        std::cout << "  Nonce: " << block.getNonce() << std::endl;
    }
}



void Blockchain::saveToFile(std::string filename) 
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    // Write header
    file << "PreviousHash,Hash,Data,Index,Nonce\n";

    // Write data
    for (auto& block : _blocks) {
        std::stringstream ss;
        ss << block.getPrevHash() << "," << block.getHash() << "," << block.getData() << ","
            << block.getIndex() << "," << block.getNonce() << "\n";
        file << ss.str();
    }

    file.close();
}

void Blockchain::appendToFile(std::string filename, Block block) 
{
    std::ofstream file(filename, std::ios_base::app);
    if (!file.is_open()) 
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::stringstream ss;
    ss << block.getPrevHash() << "," << block.getHash() << "," << block.getData() << ","
        << block.getIndex() << "," << block.getNonce() << "\n";
    file << ss.str();

    file.close();
}

void Blockchain::loadFromFile(std::string filename) 
{

    std::ifstream file(filename);
    std::string line;
    if (!file.is_open()) 
    {
        std::ofstream newFile(filename); 
        newFile << "PreviousHash,Hash,Data,Index,Nonce\n";
        newFile.close();
        return;
    }

    // Read Blocks
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prevHash, hash, data;
        int index, nonce;
        char comma;

        if (std::getline(ss, prevHash, ',') &&
            std::getline(ss, hash, ',') &&
            std::getline(ss, data, ',') &&
            ss >> index >> comma >> nonce) {
            _blocks.push_back({ prevHash, hash, data, index, nonce });
        }
        else {
            std::cerr << "Error parsing line: " << line << std::endl;
        }
    }

    file.close();
}