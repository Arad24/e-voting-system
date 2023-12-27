#include "RequestHandler.h"

RequestHandler::RequestHandler(Peer* peer)
{
    _peer = peer;
}

std::string RequestHandler::handleRequest(const std::string& request)
{
	return std::string();
}

void RequestHandler::setPeerReference(Peer* peer)
{
}

void RequestHandler::handleAddBlock(const Message& msg)
{
}

void RequestHandler::handleShareKey(const Message& msg)
{
}

void RequestHandler::handleSendMessage(const Message& msg)
{
}

void RequestHandler::handleGenericMessage(const Message& msg)
{
}
