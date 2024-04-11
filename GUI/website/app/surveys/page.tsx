'use client'

import { useEffect, useState } from 'react';
import Link from 'next/link';
import {
  getAllSurveys
} from '../dbApiConnector'

export default function SurveyList() {

  const [surveys, setSurveys] = useState<any[]>([]);

  useEffect(() => {
    async function fetchSurveys() {
      try {
        const surveyData = await getAllSurveys();
        setSurveys(surveyData); 
      } catch (error) {
        console.error('Error fetching surveys:', error);
      }
    }


    fetchSurveys();
  }, []);


  return (
    <div>
      <DefaultHeader />
      <h1 className='text-center mt-5 text-2xl font-bold'>All Surveys</h1>
      <ul>
        {surveys.map((survey, index) => (
          <li key={index} className='mt-10'>
            <SurveyCard surveyName={survey.SURVEY_NAME} surveyUid={survey.SURVEY_UID}/>
          </li>
        ))}
      </ul>
    </div>
  );
}

const SurveyCard = ({surveyName, surveyUid} : {surveyName: string, surveyUid: string}) => 
{
    return (
        <div className="bg-slate-100 border-blue-500 border-t-2 rounded-md mx-10 text-center py-5">
          <h1 className='text-lg font-semibold'>Survey Question</h1>
          <p className='text-sm mb-5'>{surveyName}</p>
          <h1 className='text-lg font-semibold'>Survey Code</h1>
          <p className='text-sm mb-5'>{surveyUid}</p>
          <Link href={`/surveys/${surveyUid}`} legacyBehavior>
            <a  className="bg-green-500 rounded-md px-10 py-0.5 hover:bg-green-400">Join</a>
          </Link>
          
        </div>
    );
}

const DefaultHeader = () => {
  return (
    <div className="bg-slate-100 p-8 text-center">
      <Link href="/" legacyBehavior>
        <a className="text-3xl hover:text-2xl">Election</a>
      </Link>
    </div>
  );
}