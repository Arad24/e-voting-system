#pragma once
# include <iostream>
# include <vector>
# include "Serializer.h"
# include "Deserializer.h"

class IRequestHandler;

struct RequestResult
{
	std::string buffer;
	std::shared_ptr<IRequestHandler> newHandler;
};

struct RequestInfo
{
	char id;
	std::vector<unsigned char> buffer;
};

class IRequestHandler
{
	public:
		virtual bool isRequestRelevant(RequestInfo rInfo) = 0;
		virtual RequestResult handleRequest(RequestInfo rInfo) = 0;
};