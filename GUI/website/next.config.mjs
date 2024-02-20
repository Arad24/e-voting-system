/** @type {import('next').NextConfig} */
import startListening from './websocket.js';

const nextConfig = 
{
  env: 
  {
    SHARE_KEY_CODE: "100",
    SHARE_KEY_CODE: "101",
    GET_PEERS_LIST_REQ: "102",
    GET_BLOCKCHAIN: "103",
    LOGIN_CODE_REQ: "104",
    LOGIN_SUCCEEDED_CODE: "204",
    ADD_VOTE_CODE: "300",
    COUNT_VOTES_CODE: "301",
    LOGIN_FAILED_CODE: "404",
  },
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