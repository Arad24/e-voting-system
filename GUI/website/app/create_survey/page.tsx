'use client'
import { useEffect, useState } from 'react';
import {
    addNewSurvey,
    generateUniqueSurveyId,
} from '../dbApiConnector'
import { useGlobalStore } from '../globals';
import Link from 'next/link';
import { useRouter } from 'next/navigation';


export default function Page() 
{
  const [surveyName, setSurveyName] = useState('');
  const [surveyOptions, setSurveyOptions] = useState(['', '']); 
  const { global_username } = useGlobalStore();
  const router = useRouter();


  const handleOptionChange = (index : any, value : any) => {
    const updatedOptions = [...surveyOptions];
    updatedOptions[index] = value;
    setSurveyOptions(updatedOptions);
  };

  const addOption = () => {
    if (surveyOptions.length < 5) {
      setSurveyOptions([...surveyOptions, '']);
    }
  };

  const handleCreateSurvey = async () => 
  {
    var uid = await createSurvey(surveyName, surveyOptions);
    if (uid == '')
    {
      alert('Error creating the survey');
    }
    else
    {
      alert('Survey created');
      router.push(`./surveys/${uid}`);
    }
    
  };

  const removeOption = (index: number) => {
    if (index >= 2 && index < 5) {
      const updatedOptions = surveyOptions.filter((_, i) => i !== index);
      setSurveyOptions(updatedOptions);
    }
  };

  return (
    <div className="text-center">
      <DefaultHeader />
      <div className="bg-slate-100 m-16 p-5 rounded-md border-t-2 border-blue-500 text-center min-xl:mx-80 lg:mx-72">
        <div className="card-header">
          <h5>Create Survey</h5>
        </div>
        <div className="card-body">
          <div className="form-group">
            <label htmlFor="surveyName"></label>
            <input
              type="text"
              placeholder = "Survey Name"
              className="my-2"
              value={surveyName}
              onChange={(e) => setSurveyName(e.target.value)}
            />
          </div>
          <div className="form-group">
            <label htmlFor="surveyOptions">Survey Options:</label>
            {surveyOptions.map((option, index) => (
              <div key={index} className="d-flex mb-2">
                <input
                  type="text"
                  className="form-control mr-2"
                  placeholder={`Option ${index + 1}`}
                  value={option}
                  onChange={(e) => handleOptionChange(index, e.target.value)}
                />
                {(index >= 2 && index < 5) && (
                  <button className='bg-red-500 px-2 mx-2 rounded-md' onClick={() => removeOption(index)}>Remove</button>
                )}
              </div>
            ))}
            {surveyOptions.length < 5 && (
              <button className="bg-green-400 px-2 rounded-2xl text-white text-xl mt-4" onClick={addOption}>
                +
              </button>
            )}
          </div>
        </div>
        <div>
          <button onClick={handleCreateSurvey} className="bg-green-400 px-2 py-1 mt-4 rounded-md text-white">Create Survey</button>
        </div>
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

async function createSurvey(surveyName: string, surveyOptions: string[])
{
    try 
    {
        const surveyUid = await generateUniqueSurveyId();
        const survey = await addNewSurvey(surveyUid, surveyName, JSON.stringify(surveyOptions));
        if (survey)
        {
          return surveyUid;
        }
        else return '';
    }
    catch(e : any)
    {
        console.log(e);
        return '';
    }
}