# pragma once
# include <iostream>
# include "LoginRequestHandler.h"
# include "MenuRequestHandler.h"

class RequestHandlerFactory
{
	public:
		RequestHandlerFactory(std::shared_ptr<IDatabase> db);

		// Login
		std::shared_ptr<LoginRequestHandler> createLoginRequestHandler();
		LoginManager& getLoginManager();

		// Menu & Room
		std::shared_ptr<MenuRequestHandler> createMenuRequestHandler(std::string username);
		RoomManager& getRoomManager();

		// Room
		// Maybe bonus: std::shared_ptr<RoomAdminRequestHandler> createRoomAdminRequestHandler(Room room, LoggedUser user);
		std::shared_ptr<RoomMemberRequestHandler> createRoomMemberRequestHandler(Room room, LoggedUser user);

	private:
		std::shared_ptr<IDatabase> m_database;
		LoginManager m_loginManager;
		RoomManager m_roomManager;
};