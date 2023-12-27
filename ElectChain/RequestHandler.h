#pragma once
#include <string>
#include "Peer.h"
#include "Block.h"
#include "Blockchain.h"
#include "Serializer.h"
#include "Deserializer.h"

class RequestHandler {
public:
    RequestHandler(Peer* peer);
    std::string handleRequest(const std::string& request);

    static void setPeerReference(Peer* peer);
    static void handleAddBlock(const Message& msg);
    static void handleShareKey(const Message& msg);
    static void handleSendMessage(const Message& msg);
    static void handleGenericMessage(const Message& msg);
private:
    static Peer* _peer;

};