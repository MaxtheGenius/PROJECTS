# A chatbot to provide customer support for an e-commerce website using NLP

import spacy  # For Natural Language Processing (spaCy)
import nltk  # For text preprocessing and tokenization
import random  # For generating random responses
import time  # For simulating processing delays

# Load the spaCy language model for English
nlp = spacy.load('en_core_web_sm')

# Sample responses for different intents
responses = {
    "greeting": ["Hello! How can I assist you today?", "Hi! How can I help you?", "Hello! How can I serve you?"],
    "order_status": [
        "Please provide your order number to check the status.",
        "I need your order number to track the status. Could you share that with me?"
    ],
    "product_inquiry": [
        "What product are you looking for?",
        "Please provide more details about the product you're looking for."
    ],
    "return_policy": [
        "Our return policy allows returns within 30 days of purchase. Would you like to initiate a return?",
        "You can return products within 30 days of receiving them. How can I assist with your return?"
    ],
    "thank_you": ["You're welcome!", "Happy to help!", "Glad I could assist you!"],
    "unknown": ["Sorry, I didn't understand that. Can you please rephrase?", "I'm not sure how to help with that."]
}

# Function to process user input using spaCy
def process_input(user_input):
    """
    Process the user input using spaCy's NLP model to extract intents and entities.
    
    Args:
        user_input (str): The input message from the user.
    
    Returns:
        intent (str): The intent identified in the user input.
        entities (list): A list of extracted entities from the user input.
    """
    doc = nlp(user_input.lower())
    
    # Define some keywords for different intents
    greeting_keywords = ["hello", "hi", "hey", "greetings"]
    order_keywords = ["order", "status", "track"]
    product_keywords = ["product", "find", "search"]
    return_keywords = ["return", "refund", "policy"]

    # Extract the intent based on keywords
    intent = "unknown"  # Default intent
    for token in doc:
        if token.text in greeting_keywords:
            intent = "greeting"
            break
        elif token.text in order_keywords:
            intent = "order_status"
            break
        elif token.text in product_keywords:
            intent = "product_inquiry"
            break
        elif token.text in return_keywords:
            intent = "return_policy"
            break
    
    # Extract entities (e.g., order numbers, product names)
    entities = [ent.text for ent in doc.ents]
    
    return intent, entities

# Function to respond to user queries
def generate_response(intent):
    """
    Generate an appropriate response based on the detected intent.
    
    Args:
        intent (str): The intent identified in the user input.
    
    Returns:
        response (str): A response message for the user.
    """
    if intent in responses:
        # Randomly choose a response from the list of responses for the given intent
        return random.choice(responses[intent])
    else:
        return random.choice(responses["unknown"])

# Function to simulate a chatbot session
def chat_with_bot():
    """
    Simulates a conversation with the chatbot by processing user inputs
    and generating responses.
    """
    print("Welcome to the Customer Support Chatbot! Type 'exit' to end the chat.")
    
    while True:
        # Get user input
        user_input = input("You: ").strip()
        
        if user_input.lower() == "exit":
            print("Goodbye! Have a great day.")
            break
        
        # Process user input and extract intent and entities
        intent, entities = process_input(user_input)
        
        # Generate and print the response based on the intent
        response = generate_response(intent)
        
        # Simulate a slight delay in response to mimic human-like interaction
        time.sleep(1)
        
        print(f"Bot: {response}")

# Main Execution
if __name__ == "__main__":
    chat_with_bot()