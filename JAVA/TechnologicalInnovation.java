import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.util.Scanner;
import com.google.gson.Gson;
import com.google.gson.JsonObject;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.entity.StringEntity;

/*
 * AIChatBot.java
 * This Java application implements a simple AI-based chatbot using OpenAI's API.
 * The chatbot responds to user input in a conversational manner.
 * It integrates a basic HTTP client and JSON parsing using Gson.
 */
public class AIChatBot {

    // The API Key for OpenAI API. Replace with your actual API key.
    private static final String API_KEY = "YOUR_OPENAI_API_KEY";

    // Method to send a request to OpenAI API and get a response
    private static String getChatResponse(String userInput) throws IOException {
        String url = "https://api.openai.com/v1/completions"; // OpenAI API endpoint

        // Create HTTP client and POST request
        CloseableHttpClient client = HttpClients.createDefault();
        HttpPost httpPost = new HttpPost(url);

        // Set HTTP headers
        httpPost.setHeader("Content-Type", "application/json");
        httpPost.setHeader("Authorization", "Bearer " + API_KEY);

        // Create the request body for the OpenAI API
        JsonObject body = new JsonObject();
        body.addProperty("model", "text-davinci-003");
        body.addProperty("prompt", userInput);
        body.addProperty("max_tokens", 100);
        body.addProperty("temperature", 0.7);

        // Convert the body to a string entity
        StringEntity entity = new StringEntity(body.toString(), StandardCharsets.UTF_8);
        httpPost.setEntity(entity);

        // Execute the request and capture the response
        String response = client.execute(httpPost, httpResponse -> {
            BufferedReader reader = new BufferedReader(new InputStreamReader(httpResponse.getEntity().getContent()));
            StringBuilder responseBuilder = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                responseBuilder.append(line);
            }
            return responseBuilder.toString();
        });

        client.close(); // Close the HTTP client
        return response;
    }

    // Method to parse the JSON response and extract the chatbot's reply
    private static String parseResponse(String response) {
        Gson gson = new Gson();
        JsonObject jsonResponse = gson.fromJson(response, JsonObject.class);
        return jsonResponse.getAsJsonArray("choices").get(0).getAsJsonObject()
                .get("text").getAsString().trim();
    }

    // Method to handle interaction with the user
    private static void startChat() {
        Scanner scanner = new Scanner(System.in);

        // Greet the user
        System.out.println("Welcome to the AI-based chatbot! Type 'exit' to quit.");

        // Main loop for chatting with the bot
        while (true) {
            System.out.print("You: ");
            String userInput = scanner.nextLine();

            // Exit condition for the chat
            if (userInput.equalsIgnoreCase("exit")) {
                System.out.println("Goodbye!");
                break;
            }

            try {
                // Get the chatbot's response
                String response = getChatResponse(userInput);
                String botReply = parseResponse(response);

                // Print the bot's reply
                System.out.println("Bot: " + botReply);
            } catch (IOException e) {
                System.err.println("Error: " + e.getMessage());
            }
        }

        scanner.close(); // Close the scanner
    }

    // Main method to start the chatbot
    public static void main(String[] args) {
        startChat(); // Start the user interaction
    }
}