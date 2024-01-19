#pragma once
#include "IRequestHandler.h"

class LoginRequestHandler : public IRequestHandler
{
    public:
        LoginRequestHandler(RequestHandlerFactory& handlerFactory);

        bool isRequestRelevant(RequestInfo rInfo) override;
        RequestResult handleRequest(RequestInfo rInfo) override;

    private:
        RequestResult login(RequestInfo rInfo);
        RequestResult signup(RequestInfo rInfo);

    RequestHandlerFactory& m_handlerFactory;
};