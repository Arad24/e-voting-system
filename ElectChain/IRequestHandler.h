#pragma once
# include "Peer.h"
# include "Deserializer.h"
# include <iostream>
# include <vector>

class Peer;
class Deserializer;
class IRequestHandler;
struct Message;

struct RequestResult
{
	std::string response;
};

struct RequestInfo
{
	char id;
	std::vector<unsigned char> buffer;
};

class IRequestHandler
{
	public:
		virtual bool isRequestRelevant(Message request) = 0;
		virtual RequestResult handleRequest(Message request) = 0;
};