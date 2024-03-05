'use client'
import React from 'react';
import Image from 'next/image';
import Logo from "../../logo.png";
import { useRouter } from 'next/navigation';
import {
  doesUserExist,
  addNewUser,
  generateUniqueUserId,
  getUidByUsername
} from '../dbApiConnector.js';
import { useGlobalStore } from '../globals';
import Link from 'next/link';


const LoginForum = () => {
  const [user_name, setName] = React.useState('');
  const [user_password, setPassword] = React.useState('');
  const {global_username, setUsername} = useGlobalStore();
  const router = useRouter();
  
  const handleSumbit = async (e: React.FormEvent<HTMLFormElement>) => {
    e.preventDefault();

    handleAddUser(user_name, user_password).then(async (result) => {
       if (result)
       {
        try {
          alert('Login succeeded');
          setUsername(user_name);
          localStorage.setItem('username', user_name);
          router.push('./');
        } catch (error) {
          console.error('Error setting global UID:', error);
        }
       } 
       else alert('Error occured');
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
    <div className="bg-slate-100 p-8 text-center">
      <Link href="/" legacyBehavior>
        <a className="text-3xl hover:text-2xl">Election</a>
      </Link>
    </div>
  );
}

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