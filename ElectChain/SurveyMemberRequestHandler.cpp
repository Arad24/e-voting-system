# include "SurveyMemberRequestHandler.h"

SurveyMemberRequestHandler::SurveyMemberRequestHandler(RequestHandlerFactory& handlerFactory, SurveyManager& surveyManager, Survey survey, LoggedUser user) :
	_handlerFactory(handlerFactory), _surveyManager(surveyManager), _survey(survey), _user(user) 
{
	// Create peer and thread detach
}

bool SurveyMemberRequestHandler::isRequestRelevant(RequestInfo rInfo)
{
	return rInfo.id >= GETSURVEYSTATS_CODE && rInfo.id <= VOTE;
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
		case VOTE:
			res = vote(rInfo);
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
		GetStatsRes = { SUCCESS, BlockchainUtils::countVotes(_peer->getBlockchain())};
		return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
	}
	catch (std::exception& e)
	{
		ErrorResponse res = { e.what() };
		return { JsonResponsePacketSerializer::SerializeResponse(res),  _handlerFactory.createMenuRequestHandler(_user.getUsername()) };
	}
}


RequestResult SurveyMemberRequestHandler::vote(RequestInfo rInfo)
{
	try
	{
		VoteRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(rInfo.buffer);

		/*
			TODO:
			check user didn't vote already
			get pk
			sign vote
			create block
			distribute the block
		*/
		GetStatsRes = { SUCCESS };
		return { JsonResponsePacketSerializer::SerializeResponse(res), nullptr };
	}
	catch (std::exception& e)
	{
		ErrorResponse res = { e.what() };
		return { JsonResponsePacketSerializer::SerializeResponse(res),  _handlerFactory.createMenuRequestHandler(_user.getUsername()) };
	}
}