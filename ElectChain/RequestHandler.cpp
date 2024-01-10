#include "RequestHandler.h"
#include "Serializer.h"
#include "Deserializer.h"
#include "Blockchain.h"

#define ERROR_CODE 22
#define ADD_BLOCK_CODE 23
#define SHARE_KEY_CODE 24
#define VOTE_BLOCK_CODE 25


RequestHandler::RequestHandler(Peer* peer)
{
    _peer = peer;
}

std::string RequestHandler::handleRequest(const Message& request) 
{
    switch (request.id) 
    {
    case ADD_BLOCK_CODE: 
    {
        Block block = Deserializer::deserializeMessageBlock(request.buffer);
        return handleAddBlock(block);
    }
    case SHARE_KEY_CODE: 
    {
        ShareKeyRequest shareKeyRequest = Deserializer::deserializeShareKey(request.buffer);
        return handleShareKey(shareKeyRequest);
    }
    case VOTE_BLOCK_CODE:

    default:
        return handleErrorResponse();
    }
}


std::string RequestHandler::handleAddBlock(const Block& block) 
{
    if () 
    {
        _peer->getBlockchain().addBlock(newBlock);
        _peer->sendBroadcastMsg("BLOCKCHAIN_UPDATED");

    return Serializer::serializeMessage({ "Response for AddBlock", ADD_BLOCK_CODE, {}, 123 });
}

std::string RequestHandler::handleShareKey(const ShareKeyRequest& shareKeyRequest) {

    return Serializer::serializeMessage({ "Response for ShareKey", SHARE_KEY_CODE, {}, 456 });
}

std::string RequestHandler::handleErrorResponse() {

    return Serializer::serializeErrorResponse({ "Unknown request type or error" });
}