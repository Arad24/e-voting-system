import { WebSocketServer } from 'ws';

const wss = new WebSocketServer({ port: 8081 });

export function startListening()
{
  wss.on('connection', function connection(ws) {
    ws.on('error', console.error);
  
    ws.on('message', function message(data) {
      console.log('received: %s', data);
    });
  
    ws.send('something');
    console.log('new connection')
  });
}
