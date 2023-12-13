# include "Blockchain.h"


void Blockchain::createNewBlock(std::string data)
{
	Block newBlock = Block(data);
}

bool Blockchain::validateBlock(Block currentBlock, int targetZeros, std::chrono::system_clock::time_point maxTimestamp) 
{
    if (_blocks.empty()) {
        std::cout << "Blockchain is empty. Cannot validate block." << std::endl;
        return false;
    }

    Block previousBlock = _blocks.back();

    // Validate the current block using the previous block
    if (currentBlock.getPrevHash() != previousBlock.getHash()) {
        std::cout << "Previous hash does not match." << std::endl;
        return false;
    }

    // Validate proof-of-work
    if (!BlockchainUtils::isValidProofOfWork(currentBlock.getHash(), targetZeros)) {
        std::cout << "Invalid proof-of-work in the block." << std::endl;
        return false;
    }
    std::chrono::system_clock::time_point currentTimestamp = std::chrono::system_clock::from_time_t(std::stoi(currentBlock.getTimestamp()));

    // Validate timestamp
    if (currentTimestamp > maxTimestamp) {
        std::cout << "Block timestamp is too far in the future." << std::endl;
        return false;
    }
    return true;
}