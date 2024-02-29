'use client'
import React from 'react';
import Image from 'next/image';
import Logo from "../../logo.png";
import { useRouter } from 'next/navigation';
import {
  doesUserExist,
  doesPasswordMatch,
  addUserToSurvey,
  getUidByUsername,
  getUsersList,
  getNodeListForSurvey,
  doesSurveyExist,
  doesSurveyIdExist,
  doesUserIdExist,
  addNewUser,
  createNewSurvey,
  isNewSurveyByUid,
  getPeerByUid,
  getPeersList,
  changePeerByUid,
  generateUniqueSurveyId,
  generateUniqueUserId
} from '../dbApiConnector.js';



const LoginForum = () => {
  const [user_name, setName] = React.useState('');
  const [user_password, setPassword] = React.useState('');
  const [db, setDb] = React.useState(null);
  const router = useRouter();
  
  const handleSumbit = async (e: React.FormEvent<HTMLFormElement>) => {
    e.preventDefault();

    handleAddUser(user_name, user_password).then((result) => {
       if (result)
       {
        alert('User added'); 
        router.push('./');
       } 
      });
  };

  return (
    <nav className="flex items-center justify-center min-h-screen">
      <div className="text-center bg-white p-14 shadow-md rounded-md sm:max-w-lg mx-16">
        <h2 className="text-3xl">Sign up</h2>
        <form onSubmit={handleSumbit}>
          <input
            type="text"
            placeholder="Enter your username"
            onChange={(e) => setName(e.target.value)}
            className="rounded px-16 py-2 mt-8 border-2 border-gray-300"
            required
          />
          <br />
          <input
            type="password"
            placeholder="Enter your password"
            onChange={(e) => setPassword(e.target.value)}
            className="rounded px-16 py-2 my-7 border-2 border-gray-300"
            required
          />
          <br />
          <button
            type="submit"
            className="bg-blue-500 text-white py-2 px-32 rounded-md text-center hover:bg-gradient-to-r from-blue-500 to-blue-400"
          >
            Submit
          </button>
        </form>
        <p className="mt-3">
        Already have an account? <a className="text-blue-900 underline" href="/login">Log in</a>
        </p>
      </div>
    </nav>
  );
};




export async function handleAddUser(username: string, password: string) {
  try {
      const userExists = await doesUserExist(username);
      if (userExists) 
      {
        alert('User already exist');
        return false;
      }

      const userUid = await generateUniqueUserId();
      const userAdded = await addNewUser(username, password, userUid, 'None');
      return userAdded;
  } catch (error) {
      console.error('Error adding user:', error);
      return false;
  }
}


const HomeBarLink = () => {
  return (
    <div>
      <a href="./">
        <Image src={Logo} alt="My Image" width={100} height={100} className="cursor-pointer" />
      </a>
    </div>
  );
};

const MyNavBar = () => {
  return (
    <div className="bg-white sticky shadow-md border-blue-500">
      <div className="flex justify-center items-center h-16">
        <HomeBarLink />
      </div>
    </div>
  );
};

export default function Login() 
{
  return (
    <main className="bg-slate-200 w-screen h-screen">
      <div className="w-screen h-16 fixed">
        <MyNavBar/>
      </div>
      <div>
        <LoginForum/>
      </div>
    </main>
  );
}