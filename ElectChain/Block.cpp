# include "Block.h"

Block::Block(std::string prevHash, std::string hash, std::string data, std::string timestamp)
{
	this->_hash = hash;
	this->_prevHash = prevHash;
	this->_data = data;
	this->_timestamp = getCurrentTimestamp();
	this->_nonce = 0;
}

Block::Block()
{
	createBlock();
}

void Block::createBlock()
{
	this->_prevHash = DEFAULT_HASH;
	this->_data = "";
	this->_timestamp = getCurrentTimestamp();
	mineHash();
}

Block::Block(std::string data)
{
	this->_data = data;
	this->_prevHash = DEFAULT_HASH;
	this->_timestamp = getCurrentTimestamp();
	mineHash();
}

std::string Block::getCurrentTimestamp()
{
	auto now = std::chrono::system_clock::now();
	std::time_t timestamp = std::chrono::system_clock::to_time_t(now);
	return ctime(&timestamp);
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

std::string Block::getTimeStamp()
{
	return this->_timestamp;
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

std::string Block::blockToStr()
{
	return _prevHash + _data + _timestamp + std::to_string(_nonce);
}
