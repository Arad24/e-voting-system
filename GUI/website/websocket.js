import { WebSocketServer } from 'ws';
import { handleRequest } from './Handler.js';

let wss;
const connectionsMap = new Map();

function startListening() {
  try {
    wss = new WebSocketServer({ port: 8881 });
    console.log('WebSocket server started on port 8881');
  } catch (error) 
  {
    console.error('Error starting WebSocket server:', error);
    return;
  }

  wss.on('connection', function connection(ws) 
  {
    const connectionId = generateUniqueId();
    connectionsMap.set(connectionId, ws);
    console.log(`New connection established with id ${connectionId}`);
    ws.on('error', console.error);

    ws.on('message', function message(data) 
    {
      data = convertDataToString(data);
      if (typeof data === 'string')
      {
        console.log(`received from connection ${connectionId}: ${data}`);
        var res = handleRequest(data)
      }
      ws.send('404{}');
    });
    
    ws.on('close', function close() 
    {
      closeSocket(connectionId)
    });

    
  });
}

function convertDataToString(data)
{
  if (Buffer.isBuffer(data)) 
  {
    data = data.toString('utf-8');
  }
  return data;
}

function closeSocket(connectionId)
{
  console.log(`Connection ${connectionId} closed`);
      connectionsMap.delete(connectionId);
}

function generateUniqueId() 
{
  return Math.random().toString(36).substring(2, 10); // Example of a simple unique ID
}

export default startListening;