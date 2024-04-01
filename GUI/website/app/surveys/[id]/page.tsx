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
    const [surveyRes, setSurveyRes] = useState({});
    const [clickOption, setClickOption] = useState(1);
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
                    if (String(answer).startsWith('601')) setAlreadyVote(true);
                    else if (String(answer).startsWith('602')) setAlreadyVote(false);
                    
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
            if (!userUid || userUid =='') userUid = await getUidByUsername(global_username);
            if (surveyUid && userUid != '')
            {
                console.log(2)
                const answer : string = String(await sendQueryAndGetRes(userUid, "301", `{"survey_uid" : "${surveyUid}"}`));
                if (answer.startsWith("501"))
                {
                    const jsonString = answer.substring(answer.indexOf('{'));
                    const parsedRes = JSON.parse(jsonString);
                    setSurveyRes(parsedRes);
                } 
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
            
            <div className='mx-20 mt-20'>
                {clickOption == 1 ? (
                    <div>
                        <button className='text-blue-600 mr-6 border-b-blue-600 border-b-2' onClick={() =>setClickOption(1)}>Survey Info</button>
                        {alreadyVote && <button onClick={() =>setClickOption(2)}>Survey Results</button>}
                        <hr className='border-b mt-2'/>

                        <div className='flex justify-between mx-10'>
                            <div>
                                <h2 className='font-bold text-xl'>Question</h2>
                                <h3>{surveyData.SURVEY_NAME}</h3>
                            </div>

                            <div className='flex items-end'>
                                <div className='flex flex-col'>
                                    <h2 className='font-bold text-xl'>Options</h2>
                                    {parsedOptions.map((option, index) => (
                                    <p key={index} value={option}>> {option}</p>
                                    ))}
                                </div>
                            </div>
                        </div>

                        <div className='mx-10'>
                            <div>
                                <h2 className='font-bold text-xl'>Status</h2>
                                <h3>{alreadyVote ? ('Got your vote') : ('Waiting for your vote')}</h3>
                            </div>

                        </div>

                        
                    </div>
                    
                ) : (
                    <div>
                        <button className='mr-6' onClick={() =>setClickOption(1)}>Survey Info</button>
                        {alreadyVote && <button className='text-blue-600 border-b-blue-600 border-b-2' onClick={() =>setClickOption(2)}>Survey Results</button>}

                        {(surveyRes && Object.keys(surveyRes).length > 0) ? (
                        <div className='text-center'>
                            <h2>Vote Results</h2>
                            <ul>
                                {Object.entries(surveyRes).map(([key, value]) => (
                                <li key={key}>{key} : {value} votes</li>
                                ))}
                            </ul>
                        </div>
                        ):
                        (
                            <div className='text-center'>
                                <button onClick={handleShowResults} className='bg-blue-500 my-2 py-2 px-5 rounded-md hover:bg-blue-400'>Show Results</button>
                            </div>
                        )}
                    </div> 
                )}
            </div>

            <div className='text-center mt-24'>
                {alreadyVote ? (
                    <div>
                        <h1>User - '{global_username}' already voted</h1>
                    </div>
                ) : (
                    <ul>
                        <select className="mx-auto block appearance-none w-1/2 bg-white border border-gray-300 rounded-md leading-tight focus:outline-none focus:bg-white focus:border-gray-500 text-center" >
                            {parsedOptions.map((option, index) => (
                                <option key={index} value={option}>{option}</option>
                            ))}
                        </select>
                        <button onClick={() => handleButtonClick({ option: document.querySelector('select').value, surveyUid, userUid, setAlreadyVote, global_username })} className='bg-blue-500 my-2 py-2 px-5 rounded-md hover:bg-blue-400'>Submit</button>
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

async function handleButtonClick({option, surveyUid, userUid, setAlreadyVote, global_username} : {option: string, surveyUid: string, userUid: string, setAlreadyVote: Function, global_username: string})
{
    try {
        if (!userUid || userUid =='') userUid = await getUidByUsername(global_username);
        const res = await sendQueryAndGetRes(userUid, '300', `{"vote":"${option}","survey_uid":"${surveyUid}"}`);
        if (String(res).startsWith('500'))
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