#pragma once
# include <iostream>
# include <vector>


class IRequestHandler;

struct RequestResult
{
	std::vector<unsigned char> buffer;
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