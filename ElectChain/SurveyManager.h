# pragma once
# include <iostream>
# include <map>
# include <string>
# include <vector>

typedef struct Survey
{
	std::string id;
	std::string surveyOwner;
	std::string name;

	nlohmann::json toJson() const
	{
		nlohmann::json jsonData = {
			{"id", id},
			{"name", name},
			{"surveyOwner", surveyOwner}
		};
		return jsonData;
	}
} Survey;

class SurveyManager
{
	public:
		Survey createSurvey(LoggedUser owner, std::string name);
		void leaveSurvey(std::string id);
		nlohmann::json getSurveyStats(unsigned int id);

		std::vector<Survey> getSurveys();
		Survey getSurveyById(std::string id);

	private:
		std::map<std::string, Survey> m_surveys; // survey id, survey

		std::string randId();
		bool isSurveyExist(std::string id);
		Survey createSurvey(std::string name, std::string owner);
};