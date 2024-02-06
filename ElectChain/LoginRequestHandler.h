#pragma once
#include "IRequestHandler.h"

class LoginRequestHandler : public IRequestHandler
{
    public:
        LoginRequestHandler();

        bool isRequestRelevant(RequestInfo rInfo) override;
        RequestResult handleRequest(RequestInfo rInfo) override;

    private:
        RequestResult login(RequestInfo rInfo);
        RequestResult signup(RequestInfo rInfo);

};