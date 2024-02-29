'use client'
import { Inter } from "next/font/google";
import "./globals.css";
import { useGlobalState } from './globals';
import { useRouter, usePathname } from 'next/navigation';
import { useEffect } from 'react';

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
  const [global_username, setUsername] = useGlobalState("username");
  const [global_uid, setUid] = useGlobalState("uid");

  useEffect(() => {
    const localUsername = localStorage.getItem('username');
    if (localUsername !== null) setUsername(localUsername);
    
    if ((global_username === '' || global_uid === '') && (pathName.toLowerCase() !== '/signup' && pathName.toLowerCase() !== '/login' && pathName.toLowerCase() !== '/')) {
      router.push('/login');
    }
  }, [global_username, global_uid, pathName, router]);
   

  return (
    <html lang="en">
      <body className={inter.className}>{children}</body>
    </html>
  );
}
