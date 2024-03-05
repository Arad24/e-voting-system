'use client'
import { useGlobalStore } from './globals';
import { useEffect, useState } from 'react';
import Link from 'next/link';
import {
  getPeerByUid,
  getUidByUsername
} from './dbApiConnector.js';



export default function Home({ data } : {data : any}) 
{
  const { global_username, setUsername } = useGlobalStore();
  const [peer, setPeer] = useState('None');
  const [nameUid, setNameUid] = useState('');


  useEffect(() => {
    const fetchData = async (global_username: string | null, setNameUid: Function, setPeer: Function) => {
      if (global_username) {
        const uid = await getUidByUsername(global_username);
        setNameUid(uid);
        if (uid != null) {
          const peerResult = await getPeerByUid(uid);
          setPeer(peerResult);
        }
      }
    };

    fetchData(global_username, setNameUid, setPeer);
  }, [global_username]);


  const handleUpdatePeer = async () => {
    if (nameUid !== '') {
      const peerResult = await getPeerByUid(nameUid);
      setPeer(peerResult);
    }
  };


  return (
    <div className="bg-white">
      {global_username === '' && 
      <div>
        <DefaultHeader/>
        <LoginError/>
      </div>
      }
      {global_username !== '' && peer === 'None' && 
        <div>
          <DefaultHeader/>
          <PeerError updatePeer={handleUpdatePeer}/>
          
        </div>
      }
      {global_username !== '' && peer !== null && peer !== 'None' && (
        <div>
          <UserHeader username={global_username} setUser={setUsername}/>
          <UserBody username={global_username}/>
        </div>
      )}
    </div>
  );
}

const UserHeader = ({ username, setUser }: { username: string | null , setUser: (username: string | null) => void }) => {
  
  const handleLogout = () => {
    Logout(setUser);
  };

  return (
    <div className="bg-slate-100 p-8 flex justify-between">
      <ul className="flex items-center md:ml-10 lg:ml-16 xl:ml-20">
        <li>
        <Link href="/" legacyBehavior>
          <a className="text-2xl">Election</a>
        </Link>
        </li>
      </ul>
      <ul className="flex items-center md:mr-10 lg:mr-16 xl:mr-20">
        <li>
          <a className="text-xl">{username}</a>
        </li>
        <li>
          <button onClick={handleLogout} className="bg-red-500 px-2 py-0.5 text-white rounded-md mx-5 hover:bg-red-400">Logout</button>
        </li>
      </ul>
    </div>
  );
}

const Logout = (setUser: (username: string | null) => void) => 
{
  localStorage.setItem('username', '');
  setUser(null);
};

const DefaultHeader = () => {
  return (
    <div className="bg-slate-100 p-8 text-center">
      <Link href="/" legacyBehavior>
        <a className="text-3xl hover:text-2xl">Election</a>
      </Link>
    </div>
  );
}

const UserBody = ({ username }: { username: string | null }) => {
  return (
    <div>
      <div className="bg-slate-100 align-top mt-16 mx-10 px-10 py-10 rounded md text-center border-t-4 border-blue-400 md:mx-20 lg:mx-40 xl:mx-48 2xl:mx-72">
        <h1 className="text-xl mb-10">Create new survey</h1>
        <Link href="/create_survey" legacyBehavior>
          <a className="bg-blue-400 px-8 py-2 rounded-md text-white hover:bg-blue-500">Create</a>
        </Link>
        
      </div>
      <div className="bg-slate-100 align-top mt-16 mx-10 px-10 py-10 rounded md text-center border-t-4 border-blue-400 md:mx-20 lg:mx-40 xl:mx-48 2xl:mx-72">
        <h1 className="text-xl mb-10">Join into survey</h1>
        <Link href="/surveys" legacyBehavior>
          <a className="bg-blue-400 px-8 py-2 rounded-md text-white hover:bg-blue-500">Join</a>
        </Link>
      </div>
    </div>
  );
}

const LoginError = () => {
  return (
    <div className="bg-slate-100 align-top mt-16 mx-10 px-10 py-10 rounded md text-center border-t-4 border-blue-400 md:mx-20 lg:mx-30 xl:mx-40 xxl:mx-50">
      <h1 className="text-xl">Error</h1>
      <h2 className="mb-10">Error: You need to login first</h2>
      <Link href="/surveys" legacyBehavior>
        <a className="bg-blue-400 px-8 py-2 rounded-md text-white">Login</a>
      </Link>
      
    </div>
  );
}

const PeerError = ({ updatePeer }: { updatePeer: () => void }) => { 
  return (
    <div className="bg-slate-100 align-top mt-16 mx-10 px-10 py-10 rounded md text-center border-t-4 border-blue-400 md:mx-20 lg:mx-30 xl:mx-40 xxl:mx-50">
      <h1 className="text-xl">Error</h1>
      <h2 className="mb-10">Error: You need to open the peer program first</h2>
      <button onClick={updatePeer} className="text-blue-900">Try again</button>
    </div>
  );
}