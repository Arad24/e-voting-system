#include "LoginRequestHandler.h"

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& handlerFactory) : m_handlerFactory(handlerFactory) {}

bool LoginRequestHandler::isRequestRelevant(RequestInfo rInfo)
{
    return rInfo.id == LOGIN_CODE || rInfo.id == SIGNUP_CODE;
}

RequestResult LoginRequestHandler::handleRequest(RequestInfo rInfo)
{
    if (isRequestRelevant(rInfo))
    {
        RequestResult retRes;
        if (rInfo.id == LOGIN_CODE)
        {
            retRes = this->login(rInfo);
        }
        else if (rInfo.id == SIGNUP_CODE)
        {
            retRes = this->signup(rInfo);
        }

        return retRes;
    }
    else
    {
        ErrorResponse res = { "Invalid Request" };
        return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
    }
}

RequestResult LoginRequestHandler::login(RequestInfo rInfo)
{
    try
    {
        LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(rInfo.buffer);
        m_handlerFactory.getLoginManager().login(req.username, req.password);

        LoginResponse res = { SUCCESS };
        return { JsonResponsePacketSerializer::SerializeResponse(res), m_handlerFactory.createMenuRequestHandler(req.username) };
    }
    catch (std::exception e)
    {
        ErrorResponse res = { e.what() };
        return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
    }
}

RequestResult LoginRequestHandler::signup(RequestInfo rInfo)
{
    try
    {
        SignupRequest req = JsonRequestPacketDeserializer::deserializeSignupRequest(rInfo.buffer);
        this->m_handlerFactory.getLoginManager().signup(req.username, req.password, req.email);

        SignupResponse res = { SUCCESS };
        return { JsonResponsePacketSerializer::SerializeResponse(res), m_handlerFactory.createMenuRequestHandler(req.username) };
    }
    catch (std::exception e)
    {
        ErrorResponse res = { e.what() };
        return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
    }
}
