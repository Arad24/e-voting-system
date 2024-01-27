# include "SurveyMemberRequestHandler.h"

SurveyMemberRequestHandler::SurveyMemberRequestHandler(RequestHandlerFactory& handlerFactory, SurveyManager& surveyManager, Survey survey, LoggedUser user) :
	_handlerFactory(handlerFactory), _surveyManager(surveyManager), _survey(survey), _user(user) 
{
	// Create peer and thread detach
}

bool SurveyMemberRequestHandler::isRequestRelevant(RequestInfo rInfo)
{
	return rInfo.id >= GETSURVEYSTATS_CODE && rInfo.id <= CONNECT_TO_PEERS;
}


RequestResult SurveyMemberRequestHandler::handleRequest(RequestInfo rInfo)
{
	if (isRequestRelevant(rInfo))
	{
		RequestResult res;
		switch (rInfo.id)
		{
		case GETSURVEYSTATS_CODE:
			res = getSurveyStats(rInfo);
			break;

		case LEAVESURVEY_CODE:
			res = leaveSurvey(rInfo);
			break;

		case CONNECT_TO_NETWORK:
			res = connectToNetwork(rInfo);
			break;

		default:
			ErrorResponse res = { "Invalid Request" };
			return { JsonResponsePacketSerializer::SerializeResponse(res),  _handlerFactory.createMenuRequestHandler(_user.getUsername()) };
			break;
		}

		return res;
	}
	else
	{
		ErrorResponse res = { "Invalid Request" };
		return { JsonResponsePacketSerializer::SerializeResponse(res),  _handlerFactory.createMenuRequestHandler(_user.getUsername()) };
	}
}

RequestResult SurveyMemberRequestHandler::connectToNetwork(RequestInfo rInfo)
{
	try
	{
		/*
			Get lists of adresses and try to connect to them
		*/
		ConnectRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(rInfo.buffer);

		/*
			TODO: 
			ConnectRequest
			{
				std::vector<endpoint> peers
			}
			
		*/
		for (auto peer : req.peers)
		{
			_peer->connect(peer);
		}
		
		ConnectResponse res = { SUCCESS };
		return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
	}
	catch (std::exception& e)
	{
		ErrorResponse res = { e.what() };
		return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
	}
}

RequestResult SurveyMemberRequestHandler::leaveSurvey(RequestInfo rInfo)
{
	try
	{
		_surveyManager.leaveSurvey(_survey.getID());
		_peer->closePeer();

		LeaveSurveyResponse res = { SUCCESS };
		return { JsonResponsePacketSerializer::SerializeResponse(res), _handlerFactory.createMenuRequestHandler(_user.getUsername()) };
	}
	catch (std::exception& e)
	{
		ErrorResponse res = { e.what() };
		return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
	}
}

RequestResult SurveyMemberRequestHandler::getSurveyStats(RequestInfo rInfo)
{
	try
	{
		/* 
			Create struct
			Canidate votes
			{
				std::string canidate_name;
				unsigned int votes;
				
				nlohmann::json toJson() const
				{
					nlohmann::json js = {
						{"canidate_name", canidate_name},
						{"votes", votes},
					};
					return js;
				}
			}
		
		*/
		GetStatsRes = { SUCCESS, _surveyManager.getSurveyById(_survey.getID())->getStats() };
		return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
	}
	catch (std::exception& e)
	{
		ErrorResponse res = { e.what() };
		return { JsonResponsePacketSerializer::SerializeResponse(res),  _handlerFactory.createMenuRequestHandler(_user.getUsername()) };
	}
}