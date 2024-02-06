#pragma once
# include <iostream>
# include <vector>
# include "Serializer.h"
# include "Deserializer.h"

class IRequestHandler;

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
		virtual bool isRequestRelevant(Message& request) = 0;
		virtual RequestResult handleRequest(Message& request) = 0;
};