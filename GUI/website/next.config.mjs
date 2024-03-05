/** @type {import('next').NextConfig} */
import { startListening } from './app/websocket.js';


export const config = {
  experimental: {
    api: {
      bodyParser: {
        sizeLimit: '1mb',
      },
      externalResolver: true,
    },
  },
};

const nextConfig = 
{
  env: {
    ADD_BLOCK_CODE: "100",
    SHARE_KEY_CODE: "101",
    GET_PEERS_LIST_REQ: "102",
    GET_BLOCKCHAIN: "103",
    LOGIN_CODE_REQ: "104",
    PEERS_LIST_SUCCEEDED_CODE: "202",
    GET_BLOCKCHAIN_RES: "203",
    LOGIN_SUCCEEDED_CODE: "204",
    ADD_VOTE_CODE: "300",
    COUNT_VOTES_CODE: "301",
    ERROR_ADD_VOTE: "400",
    ERROR_COUNT_VOTES: "401",
    LOGIN_FAILED_CODE: "404",
    SUCCESS_ADD_VOTE: "500",
    SUCCESS_COUNT_VOTES: "501",
  },
  webpack: (config, { isServer }) => {
    if (!isServer) 
    {

      console.log("Initializing application...");
      startListening();
    }
    return config;
  },

  pageExtensions: ['jsx', 'js', 'tsx', 'ts']
};

export default nextConfig;