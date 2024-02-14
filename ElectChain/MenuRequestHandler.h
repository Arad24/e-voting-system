#pragma once
# include "IRequestHandler.h"

class MenuRequestHandler : public IRequestHandler
{
	public:
		MenuRequestHandler(RequestHandlerFactory& handlerFactory, RoomManager& roomManager, LoggedUser user);

		bool isRequestRelevant(RequestInfo rInfo) override;
		RequestResult handleRequest(RequestInfo rInfo) override;

	private:
		LoggedUser m_user;
		RoomManager& m_roomManager;
		RequestHandlerFactory& m_handlerFactory;


		RequestResult signout(RequestInfo);
		RequestResult getRooms(RequestInfo);
		RequestResult getPlayersInRoom(RequestInfo);
		RequestResult joinRoom(RequestInfo);
		RequestResult createRoom(RequestInfo);

};