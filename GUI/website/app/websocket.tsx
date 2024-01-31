const address = 'ws://127.0.0.1:8820';
import React, { useEffect } from 'react';

let ws: WebSocket | null = null;
let isConnecting = false;


function isOpen() 
{
  if (ws != null) {
    return ws.readyState === WebSocket.OPEN;
  }
  return false;
}

export function createNewWs() {
  if (!isOpen() && !isConnecting) 
  {
    isConnecting = true;
    ws = new WebSocket(address);

    ws.onopen = () => {
      isConnecting = false;
    };

    ws.onerror = (error) => {
      console.error('WebSocket error:', error);
    };

    ws.onclose = () => 
    {};
  }
}

export function sendMsg(code: string, msg: string): Promise<string> {
  msg = `${code}{${msg}}`;

  return new Promise((resolve, reject) => 
  {
    if (isOpen()) 
    {
      ws!.send(msg);
    } 
    else 
    {
      createNewWs();
      ws!.onopen = () => {
        ws!.send(msg);
      };
    }

    ws!.onmessage = (event) => 
    {
      resolve(event.data);
    };
    

    ws!.onclose = () => 
    {
    };
  });
}