#pragma once
# include "IRequestHandler.h"
# include "Peer.h"
# include "../../UsefullFunctions/Header Files/stringFuncs.h"
# include "../../Infrastructure/Header Files/RequestHandlerFactory.h"

// Forward declarations
class RequestHandlerFactory;

class SurveyMemberRequestHandler : public IRequestHandler
{
	public:
		SurveyMemberRequestHandler(RequestHandlerFactory& handlerFactory, SurveyManager& surveyManager, Survey survey, LoggedUser user);
		bool isRequestRelevant(RequestInfo) override;
		RequestResult handleRequest(RequestInfo) override;

	private:
		Survey _survey;
		LoggedUser _user;
		SurveyManager& _surveyManager;
		RequestHandlerFactory& _handlerFactory;
		std::shared_ptr<Peer> _peer;

		RequestResult leaveSurvey(RequestInfo);
		RequestResult getSurveyStats(RequestInfo);
		RequestResult connectToNetwork(RequestInfo rInfo);
};