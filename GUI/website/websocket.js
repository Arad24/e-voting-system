import { WebSocketServer } from 'ws';

let wss; 

function startListening() {
  try 
  {
    wss = new WebSocketServer({ port: 8881 });
    console.log('WebSocket server started on port 8881');
  } 
  catch (error) {
    console.error('Error starting WebSocket server:', error);
    return; // Exit the function if an error occurs
  }
  
  wss.on('connection', function connection(ws) 
  {
    ws.on('error', console.error);

    ws.on('message', function message(data)
     {
      console.log('received: %s', data);
    });
    ws.send('something');
    console.log('new connection');
  });
}

export default startListening;