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
        /*
            TODO: Handle sign up with database - create loginManager
        */

        /*LoginResponse res = {SUCCESS_CODE};
        return { JsonResponsePacketSerializer::SerializeResponse(res), m_handlerFactory.createMenuRequestHandler(req.username) };*/
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
        /*
            TODO: Handle sign up with database - create loginManager
        */

        /*SignupResponse res = {SUCCESS_CODE};
        return { JsonResponsePacketSerializer::SerializeResponse(res), m_handlerFactory.createMenuRequestHandler(req.username) };*/
    }
    catch (std::exception e)
    {
        ErrorResponse res = { e.what() };
        return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
    }
}
