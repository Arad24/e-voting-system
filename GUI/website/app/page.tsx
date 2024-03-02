'use client'
import Image from "next/image";
import { useGlobalStore } from './globals';
import { useEffect, useState } from 'react';
import {
  getPeerByUid,
  getUidByUsername
} from './dbApiConnector.js';
import connectionsMap from './Connections'; 

export default function Home() {
  const { global_username } = useGlobalStore();
  const [peer, setPeer] = useState('None');
  const [nameUid, setNameUid] = useState('');

  useEffect(() => {
    fetchData(global_username, setNameUid, setPeer);
  }, [global_username, connectionsMap]);

  const handleUpdatePeer = async () => {
    if (nameUid !== '') {
      const peerResult = await getPeerByUid(nameUid);
      setPeer(peerResult);
    }
  };

  return (
    <div className="bg-white">
      {global_username === '' && <LoginError />}
      {global_username !== '' && peer === 'None' && <PeerError updatePeer={handleUpdatePeer} />}
      {global_username !== '' && peer !== null && peer !== 'None' && (
        <div>
          <UserHeader username={global_username}/>
          <UserBody username={global_username}/>
        </div>
      )}
    </div>
  );
}

async function fetchData(global_username: string | null, setNameUid: Function, setPeer: Function) 
{
  if (global_username) {
    const uid = await getUidByUsername(global_username);
    setNameUid(uid);
    if (uid != null) {
      const peerResult = await getPeerByUid(uid);
      setPeer(peerResult);
    }
  }
}

const UserHeader = ({ username }: { username: string | null }) => {
  return (
    <div className="bg-slate-100 p-8 flex justify-between">
      <ul className="flex items-center md:ml-10 lg:ml-16 xl:ml-20">
        <li>
          <a href="/" className="text-2xl">Election</a>
        </li>
      </ul>
      <ul className="flex items-center md:mr-10 lg:mr-16 xl:mr-20">
        <li>
          <a className="text-xl">{username}</a>
        </li>
        <li>
          <a href="/" className="bg-blue-600 px-2 pb-0.5 text-white text-2xl rounded-md mx-5">+</a>
        </li>
      </ul>
    </div>
  );
}

const UserBody = ({ username }: { username: string | null }) => {
  return (
    <div>
      <div className="bg-slate-100 align-top mt-16 mx-10 px-10 py-10 rounded md text-center border-t-4 border-blue-400 md:mx-20 lg:mx-40 xl:mx-48 2xl:mx-72">
        <h1 className="text-xl mb-10">Create new survey</h1>
        <a href="/login" className="bg-blue-400 px-8 py-2 rounded-md text-white">Create</a>
      </div>
      <div className="bg-slate-100 align-top mt-16 mx-10 px-10 py-10 rounded md text-center border-t-4 border-blue-400 md:mx-20 lg:mx-40 xl:mx-48 2xl:mx-72">
        <h1 className="text-xl mb-10">Join into survey</h1>
        <a href="/login" className="bg-blue-400 px-8 py-2 rounded-md text-white">Join</a>
      </div>
    </div>
  );
}

const LoginError = () => {
  return (
    <div className="bg-slate-100 align-top mt-16 mx-10 px-10 py-10 rounded md text-center border-t-4 border-blue-400 md:mx-20 lg:mx-30 xl:mx-40 xxl:mx-50">
      <h1 className="text-xl">Error</h1>
      <h2 className="mb-10">Error: You need to login first</h2>
      <a href="/login" className="bg-blue-400 px-8 py-2 rounded-md text-white">Login</a>
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