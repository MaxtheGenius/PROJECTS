// Node.js web application integrating multiple external APIs

const express = require('express');  // Web framework for Node.js
const axios = require('axios');  // HTTP client to make API requests
const path = require('path');  // Path utilities for file management
const bodyParser = require('body-parser');  // Middleware to parse request bodies
const app = express();

// Set up middleware
app.use(bodyParser.json());
app.use(express.static(path.join(__dirname, 'public')));

// Routes
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));  // Serve the main HTML page
});

// External API configurations (Fake Keys for Example)
const WEATHER_API_KEY = 'fakeWeatherApiKey123';  // Example weather API key
const SOCIAL_MEDIA_API_KEY = 'fakeSocialMediaApiKey456';  // Example social media API key
const MAPS_API_KEY = 'fakeMapsApiKey789';  // Example maps API key

// Fetch weather forecast for a given location
async function getWeather(location) {
    // Fake weather data for demonstration purposes
    return {
        main: {
            temp: 22,  // Temperature in Celsius
        },
        weather: [{
            description: 'Sunny',  // Weather description
        }],
    };
}

// Fetch social media recommendations (e.g., top tourist spots)
async function getSocialRecommendations(city) {
    // Fake social media data for demonstration purposes
    return {
        recommendations: ['Visit the famous XYZ Park', 'Check out ABC Museum', 'Taste the local cuisine at the DEF restaurant'],
    };
}

// Fetch map details for a given location (e.g., address, coordinates)
async function getMapDetails(address) {
    // Fake map data for demonstration purposes
    return {
        formatted_address: '123 Fake St, Imaginary City, Country',
    };
}

// API route to create a travel itinerary
app.post('/create-itinerary', async (req, res) => {
    const { destination, startDate, endDate } = req.body;  // Get destination and dates from the request body
    
    try {
        // Fetch weather information
        const weatherData = await getWeather(destination);

        // Fetch social media recommendations for the destination
        const socialData = await getSocialRecommendations(destination);

        // Fetch map details for the destination
        const mapData = await getMapDetails(destination);

        // Prepare the personalized travel itinerary
        const itinerary = {
            destination,
            startDate,
            endDate,
            weather: weatherData,
            socialRecommendations: socialData ? socialData.recommendations : [],
            map: mapData ? mapData.formatted_address : 'Location not found',
        };

        // Send back the itinerary as a response
        res.status(200).json(itinerary);
    } catch (error) {
        console.error('Error generating itinerary:', error);
        res.status(500).json({ error: 'Unable to generate itinerary' });
    }
});

// Start the web server
const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
});