# include "Blockchain.h"

std::shared_ptr<Block> Blockchain::createNewBlock(std::string data)
{
	std::shared_ptr<Block> newBlock = std::make_shared<Block>(data);
    return newBlock;
}


Blockchain::Blockchain()
{
    _chainLength = 0;
}

void Blockchain::addBlock(Block block)
{
	if (validateBlock(block))
	{
		_blocks.push_back(block);
        appendToFile(block);
        updateChainLength();
	}
}

int Blockchain::getLastIndex()
{
    int lastIndex = (_blocks.empty()) ? (0) : (getLatestBlock().getIndex());
    return lastIndex;
}

int Blockchain::getChainLength()
{
    return _chainLength;
}

void Blockchain::updateChainLength()
{
    _chainLength = _blocks.size();
}

void Blockchain::updateChain(std::vector<Block> newChain)
{
    std::string first_hash = newChain.front().getPrevHash();
    std::vector<Block> longestChain = isLongestChain(first_hash, newChain);
    if (longestChain.size() > _blocks.size()) {
        _blocks = longestChain;
        updateChainLength();
    }
}

std::vector<Block> Blockchain::isLongestChain(std::string hash, std::vector<Block> newChain)
{
    int flag = 0;
    std::vector<Block> longestChain;
    std::vector<Block> currentChain = getBlocks();

    if (currentChain.empty()) {
        return newChain;
    }

    for (auto& block : currentChain) {
        if (block.getHash() != hash) {
            longestChain.push_back(block);
        }
        if (block.getHash() == hash) {
            longestChain.push_back(block);
            flag = 1;
            break;
        }
    }
    if (flag)
    {
        longestChain.insert(longestChain.end(), newChain.begin(), newChain.end());
    }
    else
    {
        if (newChain.size() > currentChain.size())
        {
            longestChain = newChain;
        }
        else
        {
            longestChain = currentChain;
        }
    }

    return longestChain;
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



void Blockchain::saveToFile() 
{
    std::string filename = BLOCKCHAIN_FILENAME;
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

void Blockchain::appendToFile(Block block) 
{
    std::string filename = BLOCKCHAIN_FILENAME;
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

void Blockchain::loadFromFile()
{
    std::string filename = BLOCKCHAIN_FILENAME;
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