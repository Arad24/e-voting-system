#pragma once
#include <string>
#include "Peer.h"
#include "Block.h"
#include "Blockchain.h"
#include "Serializer.h"
#include "Deserializer.h"
#include "structs.h"

#define LOGIN_CODE 20
#define SIGNUP_CODE 21
#define ERROR_CODE 22
#define ADDBLOCK_CODE 23
#define SHAREKEY_CODE 24
#define SENDMSG_CODE 25
#define SEND_GEN_MSG 26

class RequestHandler {
public:
    RequestHandler(Peer* peer);
    std::string handleRequest(Message msg);

    static void setPeerReference(Peer* peer);
    static RequestResult handleAddBlock(const Message& msg);
    static RequestResult handleShareKey(const Message& msg);
    static RequestResult handleSendMessage(const Message& msg);
    static RequestResult handleGenericMessage(const Message& msg);
private:
    static Peer* _peer;

};