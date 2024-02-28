import { WebSocketServer } from 'ws';
import { handleRequest } from './Handler.js';

let wss;
const connectionsMap = new Map();

function startListening() 
{
  try {
    wss = new WebSocketServer({ port: 8881 });
    console.log('WebSocket server started on port 8881');
  } 
  catch (error) 
  {
    console.error('Error starting WebSocket server:', error);
    return;
  }

  wss.on('connection', function connection(ws) 
  {
    let connectionId = '';
    
    console.log(`New connection established `);
    ws.on('error', console.error);

    ws.on('message', function message(data) 
    {
      data = convertDataToString(data);
      var res;
      if (typeof data === 'string')
      {
        
        console.log(`data: ${data}`);
        
        res = handleRequest(data);

        if (res.startWith(process.env.LOGIN_SUCCEEDED_CODE))
        {
          
          var jsonRes = JSON.parse(data)
          connectionId = jsonRes.uid;
          connectionsMap.set(connectionId, ws);
        }
        else if (connectionsMap.has(connectionId))
        {
          ws.send(res)
        }
        else
        {
          ws.send(`${process.env.LOGIN_FAILED_CODE}{"message":"You need to login first"}`)
        } 
      }
      ws.send(res);
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

export default startListening;