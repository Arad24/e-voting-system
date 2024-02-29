'use client';
import Image from "next/image";
import { useGlobalState } from './globals';
import { useRouter } from 'next/navigation';
import {
  getPeerByUid,
  getUidByUsername
} from './dbApiConnector.js';
import { useEffect, useState } from 'react';

export default function Home() {
  const [global_username, setUsername] = useGlobalState("username");
  const [uid, setUid] = useState('');
  const [peer, setPeer] = useState("None");

  useEffect(() => {
    const fetchData = async () => 
    {
      if (global_username)
      {
        const nameUid = await getUidByUsername(global_username);
        setUid(nameUid);
        console.log(`nameUid: ${nameUid}`);
        if(nameUid != null)
        {
          const peerResult = await getPeerByUid(nameUid);
          setPeer(peerResult);
        }
      } 
      console.log(peer)
    };
    fetchData();
  });


  return (
    <div className="bg-white">
      <div className="bg-slate-100">
        <h1 className="text-4xl flex justify-center p-8">Election</h1>
      </div>
      {global_username === '' && (
        <LoginError />
      )}
      {global_username !== '' && peer == 'None' && (
        <PeerError />
      )}
      {global_username !== '' && peer !== 'None' && (
        <h1>Hello, {global_username}</h1>
      )}
    </div>
  );
}

const LoginError = () => 
{

  return (
    <div className="bg-slate-100 align-top mt-16 mx-10 px-10 py-10 rounded md text-center border-t-4 border-blue-400 md:mx-20 lg:mx-30 xl:mx-40 xxl:mx-50">
      <h1 className="text-xl">Error</h1>
      <h2 className="mb-10">Error: You need to login first</h2>
      <a href="/login" className="bg-blue-400 px-8 py-2 rounded-md text-white">Login</a>
    </div>
  );
}

const PeerError = () => 
{
  return (
    <div className="bg-slate-100 align-top mt-16 mx-10 px-10 py-10 rounded md text-center border-t-4 border-blue-400 md:mx-20 lg:mx-30 xl:mx-40 xxl:mx-50">
      <h1 className="text-xl">Error</h1>
      <h2 className="mb-10">Error: You need to open the peer program first</h2>
      <a href="/login" className="text-blue-900">Try again</a>
    </div>
  );
}