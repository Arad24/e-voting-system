# include "Block.h"

Block::Block(std::string prevHash, std::string hash, std::string data, int index, int nonce)
{
	this->_hash = hash;
	this->_prevHash = prevHash;
	this->_data = data;
	this->_index = index;
	this->_nonce = nonce;
}

Block::Block()
{
	createBlock();
}

Block::Block(std::string data)
{
	this->_data = data;
	this->_prevHash = (BlockchainUtils::_bcCopy->getBlocks().empty()) ? ("000") : (BlockchainUtils::_bcCopy->getLatestBlock().getHash());
	this->_index = BlockchainUtils::_bcCopy->getLastIndex() + 1;
	mineHash();
}


void Block::createBlock()
{
	this->_prevHash = DEFAULT_HASH;
	this->_data = "";
	this->_index = BlockchainUtils::_bcCopy->getLastIndex() + 1;
	mineHash();
}

std::string Block::getData()
{
	return this->_data;
}

std::string Block::getHash()
{
	return this->_hash;
}

std::string Block::getPrevHash()
{
	return this->_prevHash;
}

int Block::getIndex()
{
	return this->_index;
}

int Block::getNonce()
{
	return _nonce;
}

void Block::setHash(std::string hash)
{
	this->_hash = hash;
}

void Block::setIndex(int index)
{
	this->_index = index;
}

void Block::setNonce(int nonce)
{
	this->_nonce = nonce;
}

void Block::setPrevHash(std::string prevHash)
{
	this->_prevHash = prevHash;
	mineHash();
}

void Block::setData(std::string data)
{
	this->_data = data;
	mineHash();
}

void Block::mineHash()
{
	std::string blockHash = BlockchainUtils::calculateHash(blockToStr());

	while (!BlockchainUtils::isValidHash(blockHash))
	{
		_nonce++;
		blockHash = BlockchainUtils::calculateHash(blockToStr());
	}

	this->_hash = blockHash;

}

std::string Block::blockToStr()
{
	return _prevHash + _data + std::to_string(_index) + std::to_string(_nonce);
}
