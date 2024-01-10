#include "RequestHandler.h"

RequestHandler::RequestHandler(Peer* peer)
{
    _peer = peer;
}

std::string RequestHandler::handleRequest(Message msg)
{
    int messageTypeCode = msg.id;
    switch (messageTypeCode) {
    case ADDBLOCK_CODE:
        handleAddBlock(msg);
        break;
    case SHAREKEY_CODE:
        handleShareKey(msg);
        break;
    case SENDMSG_CODE:
        handleSendMessage(msg);
        break;
    case SEND_GEN_MSG:
        handleGenericMessage(msg);
        break;

    }

    return /* Some response */;
}

void RequestHandler::setPeerReference(Peer* peer)
{
	_peer = peer;
}

RequestResult RequestHandler::handleAddBlock(const Message& msg)
{
    RequestResult result;
    Block newBlock = Deserializer::deserializeMessageBlock(msg.buffer);
    if (_peer->getBlockchain().validateBlock(newBlock)) {
        _peer->getBlockchain().addBlock(newBlock);
        _peer->sendBroadcastMsg("BLOCKCHAIN_UPDATED");
        result.response =  Serializer::serializeMessageBlock(newBlock);
        return result;
    }
    else {
        ErrorResponse error;
        error.message = "Signup failed";
        result.response = Serializer::serializeErrorResponse(error);
        return result;

    }
    return result;
}

RequestResult RequestHandler::handleShareKey(const Message& msg)
{
    ShareKeyRequest sharedKey = Deserializer::deserializeShareKey(msg.buffer);
    RSA* pkey= BlockchainUtils::vectorToRSA(sharedKey.publicKey);
    std::cout << "Received public key from user " << sharedKey.userId << std::endl;
    BlockchainUtils::shareKey(pkey);
}

RequestResult RequestHandler::handleSendMessage(const Message& msg)
{
    Message receivedMessage = Deserializer::deserializeMessage(msg.buffer);
    std::string bufferString(receivedMessage.buffer.begin(), receivedMessage.buffer.end());
    _peer->sendMsg(bufferString);

}

RequestResult RequestHandler::handleGenericMessage(const Message& msg)
{
    std::string bufferString(msg.buffer.begin(), msg.buffer.end());
    std::cout << "Received a generic message " << ": " << bufferString << std::endl;
}
