import { WebSocketServer } from 'ws';
import { handleRequest } from './Handler.js';
import { changePeerByUid } from './dbApiConnector.js';

let wss;
const connectionsMap = new Map();

export default async function startListening() {
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

    ws.on('message', function message(data) {
      data = convertDataToString(data);
      var res;
      if (typeof data === 'string') {
        console.log(`data: ${data}`);
        handleRequest(data)
          .then((result) => {
            console.log(result);
            if (result && result.startsWith('204')) {
              var jsonRes = JSON.parse(result.substring(3, result.length));
              connectionId = jsonRes.uid;
              connectionsMap.set(connectionId, ws);
              ws.send(result);
            } else if (connectionsMap.has(connectionId)) {
              ws.send(result);
            } else {
              ws.send(`${process.env.LOGIN_FAILED_CODE}{"message":"You need to login first"}`);
            }
          })
          .catch((error) => {
            console.error('Error handling request:', error);
          });
      }
    });

    ws.on('close', function close() {
      closeSocket(connectionId);
    });
  });
}

export async function sendMsgToWs(ws, msg) {
  if (ws !== null) {
    ws.send(msg, (err) => {
      if (err) {
        console.error('Error sending message:', err);
      }
    });

    ws.on('message', function message(data) {
      data = convertDataToString(data);
      if (typeof data === 'string') {
        return data;
      }
    });
  } else {
    return '';
  }
}

function convertDataToString(data) {
  if (Buffer.isBuffer(data)) {
    data = data.toString('utf-8');
  }
  return data;
}

async function closeSocket(connectionId) {
  console.log(`Connection ${connectionId} closed`);
  connectionsMap.delete(connectionId);
  try {
    await changePeerByUid(connectionId, 'None');
  } catch (error) {
    console.error('Error closing socket:', error);
  }
}
