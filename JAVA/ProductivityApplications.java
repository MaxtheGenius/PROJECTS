import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.server.handler.AbstractHandler;
import org.eclipse.jetty.websocket.api.Session;
import org.eclipse.jetty.websocket.api.annotations.OnWebSocketClose;
import org.eclipse.jetty.websocket.api.annotations.OnWebSocketConnect;
import org.eclipse.jetty.websocket.api.annotations.OnWebSocketMessage;
import org.eclipse.jetty.websocket.api.annotations.WebSocket;
import org.eclipse.jetty.websocket.server.WebSocketHandler;
import org.eclipse.jetty.websocket.server.WebSocketServletFactory;

import java.io.IOException;
import java.util.Collections;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

/*
 * CollaborativeTextEditor.java
 * Implements a real-time collaborative text editor using Java and WebSockets.
 */
public class CollaborativeTextEditor {

    // Step 1: WebSocket implementation to handle client connections and messages
    @WebSocket
    public static class EditorWebSocket {

        // Thread-safe set to manage active sessions
        private static final Set<Session> sessions = Collections.newSetFromMap(new ConcurrentHashMap<>());

        // Handle new client connections
        @OnWebSocketConnect
        public void onConnect(Session session) {
            sessions.add(session);
            System.out.println("New client connected: " + session.getRemoteAddress());
        }

        // Handle client disconnections
        @OnWebSocketClose
        public void onClose(Session session, int statusCode, String reason) {
            sessions.remove(session);
            System.out.println("Client disconnected: " + session.getRemoteAddress());
        }

        // Handle messages from clients
        @OnWebSocketMessage
        public void onMessage(Session sender, String message) {
            System.out.println("Received message: " + message);

            // Broadcast message to all connected clients
            for (Session session : sessions) {
                if (session.isOpen() && session != sender) {
                    try {
                        session.getRemote().sendString(message);
                    } catch (IOException e) {
                        System.err.println("Error sending message: " + e.getMessage());
                    }
                }
            }
        }
    }

    // Step 2: WebSocket handler to register WebSocket endpoints
    public static class EditorWebSocketHandler extends WebSocketHandler {

        @Override
        public void configure(WebSocketServletFactory factory) {
            factory.register(EditorWebSocket.class);
        }
    }

    // Step 3: Main method to start the WebSocket server
    public static void main(String[] args) {
        // Define server port
        int port = 8080;

        // Create and configure the Jetty server
        Server server = new Server(port);
        server.setHandler(new EditorWebSocketHandler());

        try {
            // Start the server
            server.start();
            System.out.println("Collaborative Text Editor is running on ws://localhost:" + port);
            server.join();
        } catch (Exception e) {
            System.err.println("Server error: " + e.getMessage());
        } finally {
            try {
                server.stop();
            } catch (Exception e) {
                System.err.println("Error stopping server: " + e.getMessage());
            }
        }
    }
}