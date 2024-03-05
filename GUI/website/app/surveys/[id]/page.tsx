'use client'
import { useEffect, useState } from 'react';
import {
    getSurveyByUid,
    getUidByUsername
} from '../../dbApiConnector'
import {
    sendMsgToWs
} from '../../websocket'
import { useGlobalStore } from '../../globals';
import Link from 'next/link';

interface PostView
{
    params: {
        id: string;
    }
}

export default function Page(props: PostView) 
{
    const [surveyData, setSurveyData] = useState(null);
    const surveyUid = props.params.id;
    const { global_username } = useGlobalStore();

    useEffect(() => {
        async function fetchSurvey() {
            try 
            {
                const surveys = await getSurveyByUid(surveyUid);
                const survey = surveys.length > 0 ? surveys[0] : null;
                console.log("Fetched survey:", survey); // Log fetched survey for debugging
                setSurveyData(survey); 
            } catch (error) {
                console.error('Error fetching survey:', error);
            }
        }

        fetchSurvey();
    }, []);

    if (!surveyData) {
        return <div>Loading...</div>;
    }


    const arrayString = JSON.parse(surveyData.SURVEY_OPTIONS); // Parse the string into a valid JSON string
    const parsedOptions = JSON.parse(arrayString);

    return (
        <div>
            <DefaultHeader />       
            <div className='text-center'>
                <h1 className='text-xl font-semibold mt-5 mb-10'>{surveyData.SURVEY_NAME}</h1>
                <ul>
                    {parsedOptions.map((option, index) => (
                        <li>
                            <button className='bg-blue-500 my-2 py-2 px-5 rounded-md hover:bg-blue-400' key={index} onClick={() => handleButtonClick({option, surveyUid, global_username})}>{option}</button>
                        </li>
                        
                    ))}
                </ul>
            </div>
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

async function handleButtonClick({option, surveyUid, global_username} : {option: string, surveyUid: string, global_username: string})
{
    try {
        const userUid = await getUidByUsername(global_username);
        const res = await sendQueryAndGetRes(userUid, '300', `{"vote":"${option}","survey_uid":"${surveyUid}"}`);
        console.log(res);
    } catch (error : any) {
        throw new Error(error);
    }

    
}

export function sendQueryAndGetRes(userUid : string, userCode : string, userJson : string) {
    return new Promise((resolve, reject) => {
      const ws = new WebSocket('ws://localhost:8881');
  
      ws.onopen = () => {
        console.log('Connected to WebSocket server');
        ws.send(`idiot{"uid":"${userUid}","msg_code":"${userCode}","msg_json":${userJson}}`);
      };
  
      ws.onmessage = (message) => {
        console.log('Received message from server:', message.data);
        resolve(message.data);
        ws.close(); 
      };
  
      ws.onerror = (error) => {
        console.error('WebSocket error:', error);
        reject(error); 
      };
  
      ws.onclose = () => {
        console.log('WebSocket connection closed');
      };
    });
  }