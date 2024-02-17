/** @type {import('next').NextConfig} */
import startListening from './websocket.js';

const nextConfig = {
  webpack: (config, { isServer }) => {
    if (!isServer) 
    {

      console.log("Initializing application...");
      startListening();
    }
    return config;
  }
};

export default nextConfig;