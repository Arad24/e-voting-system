import { WebSocketServer } from 'ws';
import { handleRequest } from './Handler.js';
import { changePeerByUid } from './dbApiConnector.js';

let wss;
export const connectionsMap = new Map();

export async function startListening() 
{
  try {
    wss = new WebSocketServer({ port: 8881 });
    console.log('WebSocket server started on port 8881');
  } catch (error) {
    console.error('Error starting WebSocket server:', error);
    return;
  }

  wss.on('connection', function connection(ws) {
    let connectionId = '';
    
    console.log(`New connection established `);
    ws.on('error', console.error);

    ws.on('message', async function message(data) 
    {
      data = convertDataToString(data);
      if (typeof data === 'string') {
        if (data.startsWith('TOSERVER'))
        {
          var dataToSend = await handleSendMsgToSocket(data.substring('TOSERVER'.length));
          if (dataToSend != '') ws.send(dataToSend);
        } 
        else
        {
          handleRequest(data)
          .then((result) => {
            if (result && result.startsWith('204')) {
              var jsonRes = JSON.parse(result.substring(3));
              connectionId = jsonRes.uid;
              connectionsMap.set(connectionId, ws);
              ws.send(result);
            } 
            else if (connectionsMap.has(connectionId)) 
            {
              if (!result.startsWith('0')) ws.send(result);
            } 
            else {
              if (!result.startsWith('0')) ws.send(`${process.env.LOGIN_FAILED_CODE}{"message":"You need to login first"}`);
            }
          })
          .catch((error) => {
            console.error('Error handling request:', error);
          });
        }

      }
    });

    ws.on('close', function close() {
      closeSocket(connectionId);
    });
  });
}

export function sendMsgToWs(uid, msg) 
{
  return new Promise((resolve, reject) => 
  {
      const ws = connectionsMap.get(uid);
      if (!ws) {
          reject(new Error('WebSocket connection not found'));
          return;
      }

      ws.send(msg, (err) => {
          if (err) {
              console.error('Error sending message:', err);
              reject(err);
              return;
          }
      });

      ws.once('message', function message(data) {
          data = convertDataToString(data);
          resolve(data);
      });
  });
}

async function handleSendMsgToSocket(msg) 
{
  try 
  {
      var jsonReq = JSON.parse(msg);
      var res = await sendMsgToWs(jsonReq.uid, `${jsonReq.msg_code}${JSON.stringify(jsonReq.msg_json)}`);
      return res;
  } 
  catch (error) 
  {
      console.error('Error handling message to socket:', error);
      return '';
  }
}

function convertDataToString(data) {
  if (Buffer.isBuffer(data)) {
    data = data.toString('utf-8');
  }
  return data;
}

async function closeSocket(connectionId) 
{
  console.log(`Connection ${connectionId} closed`);
  try {
    connectionsMap.delete(connectionId);
    await changePeerByUid(connectionId, 'None');
  } catch (error) {
    console.error('Error closing socket:', error);
  }
}


