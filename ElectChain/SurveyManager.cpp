#include "SurveyManager.h"


Survey SurveyManager::createRoom(LoggedUser user, std::string name, unsigned int maxPlayers, unsigned int answerTimeOut)
{
    Survey rm = createSurvey(name, maxPlayers, answerTimeOut);
    auto room = std::make_shared<Room>(user, rm);
    this->m_surveys.insert({ rm.id, room });

    return rm;
}

void SurveyManager::leaveSurvey(std::string id)
{
    this->m_surveys.erase(id);
}

std::vector<Survey> SurveyManager::getSurveys()
{
    std::vector<Survey> surveys;

    for (auto const& i : m_surveys)
    {
        auto survey = i.second;
        surveys.emplace_back(survey);
    }

    return surveys;
}


Survey SurveyManager::getSurveyById(std::string id)
{
    auto it = this->m_surveys.find(id);

    if (it != this->m_surveys.end())
    {
        return (it->second);
    }
    else throw std::out_of_range("Room with the specified id not found");
}

bool SurveyManager::isSurveyExist(std::string roomId)
{
    return (this->m_surveys.find(roomId) != this->m_surveys.end());
}

std::string SurveyManager::randId() 
{
    bool exist = true;
    std::string id = "";

    while (exist) 
    {
        id = "";
        for (int i = 0; i < 5; i++) 
        {
            id += (rand() % 2 == 0) ? char('a' + rand() % 26) : char('0' + rand() % 10);
        }

        exist = isSurveyExist(id);
    }

    return id;
}


Survey SurveyManager::createSurvey(std::string name, std::string owner)
{
    Survey rd = Survey();

    rd.id = randId();
    rd.name = name;
    rd.surveyOwner = owner;

    return rd;
}