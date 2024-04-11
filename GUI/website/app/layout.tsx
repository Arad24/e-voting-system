'use client'
import { Inter } from "next/font/google";
import "./globals.css";
import { useGlobalStore } from './globals';
import { useRouter, usePathname } from 'next/navigation';
import { useEffect, useState } from 'react';
import {
  getUidByUsername,
  getPeerByUid
} from './dbApiConnector'

const inter = Inter({ subsets: ["latin"] });
/*
export const metadata = {
  title: 'Election',
  description: 'Arad & Alex',
}*/

export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) 
{
  const router = useRouter();
  const pathName = usePathname();
  const { global_username, setUsername } = useGlobalStore();

  useEffect(() => {
    const localUsername = localStorage.getItem('username');
    if (localUsername !== null) setUsername(localUsername);
    
    if ((global_username === '') && (pathName.toLowerCase() !== '/signup' && pathName.toLowerCase() !== '/login' && pathName.toLowerCase() !== '/')) {
      router.push('/login');
    }
    else if ((global_username !== ''))
    {
      const fetchData = async () => {
        const getUid = await getUidByUsername(global_username);
        if (getUid != null) {
          const peerResult = await getPeerByUid(getUid);
          console.log('peerResult:', peerResult);
          if (peerResult === null || peerResult === 'None' || peerResult === '') {
            console.log('Redirecting to home page');
            router.push('/');
          }
        }
      };
      fetchData();

    }
  }, [global_username]);
   

  return (
    <html lang="en">
      <body className={inter.className}>{children}</body>
    </html>
  );
}
