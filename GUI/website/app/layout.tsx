import type { Metadata } from "next";
import { Inter } from "next/font/google";
import "./globals.css";
import { startListening } from './websocket'

const inter = Inter({ subsets: ["latin"] });

export const metadata: Metadata = {
  title: "Election",
  description: "Alex & Arad",
};

export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {

  try
  {
    console.log(1);
    startListening();
  }
  catch (e)
  {
    console.log('Server closed')
  }
  return (
    <html lang="en">
      <body className={inter.className}>{children}</body>
    </html>
  );
}
