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
    const [alreadyVote, setAlreadyVote] = useState(false);
    let userUid = '';


    useEffect(() => {
        async function fetchSurvey() {
            try 
            {
                const surveys = await getSurveyByUid(surveyUid);
                const survey = surveys.length > 0 ? surveys[0] : null;
                setSurveyData(survey); 
            } catch (error) {
                console.error('Error fetching survey:', error);
            }
        }

        async function checkIfAlreadyVote() {
            try 
            {
                userUid = await getUidByUsername(global_username);
                
                if (surveyUid && userUid)
                {
                    const answer = await sendQueryAndGetRes(userUid, "600", `{"user_uid":"${userUid}", "survey_uid" : "${surveyUid}"}`);
                    console.log(answer);
                    if (answer.startsWith('601')) setAlreadyVote(true);
                    else if (answer.startsWith('602')) setAlreadyVote(false);
                    console.log(alreadyVote)
                }
                
            } catch (error) {
                console.error('Error check if already vote:', error);
            }
        }

        fetchSurvey();
        checkIfAlreadyVote();
    }, []);

    if (!surveyData) {
        return <div>Loading...</div>;
    }

    async function handleShowResults() {
        try 
        {
            if (!userUid) userUid = await getUidByUsername(global_username);
            if (surveyUid && userUid != '')
            {
                console.log(2)
                const answer = await sendQueryAndGetRes(userUid, "301", `{"survey_uid" : "${surveyUid}"}`);
                console.log(answer)
            }
            
        } catch (error) {
            console.error('Error check results:', error);
        }
    }


    const arrayString = JSON.parse(surveyData.SURVEY_OPTIONS); // Parse the string into a valid JSON string
    const parsedOptions = JSON.parse(arrayString);

    return (
        <div>
            <DefaultHeader />       
            <div className='text-center'>
                <h1 className='text-xl font-semibold mt-5 mb-10'>{surveyData.SURVEY_NAME}</h1>
                {alreadyVote ? (
                    <div>
                        <h1>User - '{global_username}' already vote</h1>
                        <button onClick={handleShowResults} className='bg-blue-500 my-2 py-2 px-5 rounded-md hover:bg-blue-400' >Show Results</button>
                    </div>
                    
                ) : (
                <ul>
                    {parsedOptions.map((option, index) => (
                        <li key={index}>
                            <button className='bg-blue-500 my-2 py-2 px-5 rounded-md hover:bg-blue-400' onClick={() => handleButtonClick({option, surveyUid, userUid})}>{option}</button>
                        </li>
                    ))}
                </ul>
            )}
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

async function handleButtonClick({option, surveyUid, userUid, setAlreadyVote} : {option: string, surveyUid: string, userUid: string, setAlreadyVote: Function})
{
    try {
        const res = await sendQueryAndGetRes(userUid, '300', `{"vote":"${option}","survey_uid":"${surveyUid}"}`);
        if (res.startWith('500'))
        {
            setAlreadyVote(true);
            alert('Vote added')
        }
    } 
    catch (error : any) {
        throw new Error(error);
    }

    
}

export function sendQueryAndGetRes(userUid: string, userCode: string, userJson: string, timeout = 5000) {
    return new Promise((resolve, reject) => {
        const ws = new WebSocket('ws://localhost:8881');
        let timeoutId: ReturnType<typeof setTimeout>;

        ws.onopen = () => {
            console.log('Connected to WebSocket server');
            const msgToSend = `TOSERVER{"uid":"${userUid}","msg_code":"${userCode}","msg_json":${userJson}}`
            console.log(msgToSend)
            ws.send(msgToSend);
            timeoutId = setTimeout(() => {
                reject(new Error('WebSocket response timeout'));
                ws.close();
            }, timeout);
        };

        ws.onmessage = (message) => {
            clearTimeout(timeoutId);
            resolve(message.data);
            ws.close();
        };

        ws.onerror = (error) => {
            console.error('WebSocket error:', error);
            clearTimeout(timeoutId);
            reject(error);
            ws.close();
        };

        ws.onclose = () => {
            console.log('WebSocket connection closed');
        };
    });
}