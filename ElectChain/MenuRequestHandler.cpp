# include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& handlerFactory, RoomManager& roomManager, LoggedUser user)
	: m_handlerFactory(handlerFactory), m_roomManager(roomManager), m_user(user)
{}


bool MenuRequestHandler::isRequestRelevant(RequestInfo rInfo)
{
	return (rInfo.id >= LOGOUT_CODE && rInfo.id <= CREATEROOM_CODE);
}

RequestResult MenuRequestHandler::handleRequest(RequestInfo rInfo)
{
	if (isRequestRelevant(rInfo))
	{
		RequestResult res;
		switch (rInfo.id)
		{
		case LOGOUT_CODE:
			res = signout(rInfo);
			break;

		case GETROOMS_CODE:
			res = getRooms(rInfo);
			break;

		case JOINROOM_CODE:
			res = joinRoom(rInfo);
			break;

		case CREATEROOM_CODE:
			res = createRoom(rInfo);
			break;

		default:
			ErrorResponse res = { "Invalid Request" };
			return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
			break;
		}

		return res;
	}
	else
	{
		ErrorResponse res = { "Invalid Request" };
		return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
	}
}


RequestResult MenuRequestHandler::signout(RequestInfo rInfo)
{
	try
	{
		m_handlerFactory.getLoginManager().logout(m_user.getUsername());

		LogoutResponse res = { SUCCESS };
		return { JsonResponsePacketSerializer::SerializeResponse(res), m_handlerFactory.createLoginRequestHandler() };
	}
	catch (std::exception& e)
	{
		ErrorResponse res = { e.what() };
		return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
	}

}


RequestResult MenuRequestHandler::getRooms(RequestInfo rInfo)
{
	try
	{
		/*
			TODO: Handle getRooms with RoomManager
		*/
	}
	catch (std::exception& e)
	{
		ErrorResponse res = { e.what() };
		return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
	}
}

RequestResult MenuRequestHandler::joinRoom(RequestInfo rInfo)
{
	try
	{
		/*
			TODO: Handle joinRoom with RoomManager
		*/

		JoinRoomResponse res = { SUCCESS };
		return { JsonResponsePacketSerializer::SerializeResponse(res), m_handlerFactory.createRoomMemberRequestHandler(*m_roomManager.getRoomById(req.roomId), m_user) };
	}
	catch (std::exception& e)
	{
		ErrorResponse res = { e.what() };
		return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
	}

}

RequestResult MenuRequestHandler::createRoom(RequestInfo rInfo)
{
	try
	{
		/*
			TODO: Create room with roomManager and create new blockchain system + start listen in new peer
		*/
	}
	catch (std::exception& e)
	{
		ErrorResponse res = { e.what() };
		return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
	}

}