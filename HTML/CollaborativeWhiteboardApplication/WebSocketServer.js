const WebSocket = require('ws');
const wss = new WebSocket.Server({ port: 8080 });

wss.on('connection', ws => {
    console.log('A user connected.');

    // Broadcast incoming drawing or doodle data to all connected clients
    ws.on('message', message => {
        const data = JSON.parse(message);

        if (data.type === 'draw') {
            // Broadcast drawing data to all clients
            wss.clients.forEach(client => {
                if (client !== ws && client.readyState === WebSocket.OPEN) {
                    client.send(JSON.stringify(data));
                }
            });
        } else if (data.type === 'randomDoodle') {
            // Broadcast random doodle to all clients
            wss.clients.forEach(client => {
                if (client !== ws && client.readyState === WebSocket.OPEN) {
                    client.send(JSON.stringify(data));
                }
            });
        }
    });

    ws.on('close', () => {
        console.log('A user disconnected.');
    });
});

console.log('WebSocket server is running on ws://localhost:8080');