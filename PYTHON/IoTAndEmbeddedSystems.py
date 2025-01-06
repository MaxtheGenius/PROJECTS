# System to Collect Sensor Data and Send to Cloud

import time  # For timing operations
import Adafruit_DHT  # Library for DHT sensors (temperature and humidity)
import requests  # For sending data to the cloud (ThingSpeak)
import random  # Simulating air quality sensor data
import json  # To handle JSON data
import logging  # For logging system activity

# Configuration
SENSOR_TYPE = Adafruit_DHT.DHT22  # DHT22 sensor for temperature and humidity
SENSOR_PIN = 4  # GPIO pin where the sensor is connected
THINGSPEAK_API_KEY = 'YOUR_THINGSPEAK_API_KEY'  # ThingSpeak API key (replace with your actual key)
THINGSPEAK_URL = f'https://api.thingspeak.com/update?api_key={THINGSPEAK_API_KEY}'

# Logging setup
logging.basicConfig(filename='environmental_monitoring.log', level=logging.INFO,
                    format='%(asctime)s - %(levelname)s - %(message)s')

# Function to read temperature and humidity from the DHT sensor
def read_temperature_and_humidity():
    """
    Reads temperature and humidity from a DHT22 sensor connected to the Raspberry Pi.
    
    Returns:
        tuple: Temperature in Celsius, Humidity in percentage
    """
    humidity, temperature = Adafruit_DHT.read_retry(SENSOR_TYPE, SENSOR_PIN)
    
    # Log the sensor reading attempt
    if humidity is not None and temperature is not None:
        logging.info(f'Read temperature: {temperature}C, Humidity: {humidity}%')
    else:
        logging.error('Failed to read from DHT sensor')
    
    return temperature, humidity

# Function to simulate air quality data (for demonstration purposes)
def read_air_quality():
    """
    Simulates air quality data (values between 0 and 100).
    
    Returns:
        float: Simulated air quality value
    """
    air_quality = random.uniform(0, 100)  # Simulate air quality between 0 (good) and 100 (poor)
    logging.info(f'Read air quality: {air_quality}')
    return air_quality

# Function to send data to ThingSpeak
def send_to_thingspeak(temperature, humidity, air_quality):
    """
    Sends sensor data (temperature, humidity, air quality) to ThingSpeak platform.
    
    Parameters:
        temperature (float): The temperature value
        humidity (float): The humidity value
        air_quality (float): The air quality value
    """
    # Prepare data to send
    data = {
        'field1': temperature,  # Temperature
        'field2': humidity,     # Humidity
        'field3': air_quality    # Air Quality
    }
    
    # Send data to ThingSpeak
    try:
        response = requests.get(THINGSPEAK_URL, params=data)
        if response.status_code == 200:
            logging.info('Data successfully sent to ThingSpeak')
        else:
            logging.error(f'Failed to send data: {response.status_code}')
    except Exception as e:
        logging.error(f'Error sending data to ThingSpeak: {e}')

# Function to collect sensor data and send it to the cloud
def collect_and_send_data():
    """
    Collects data from the environmental sensors and sends it to ThingSpeak.
    Runs in a continuous loop.
    """
    while True:
        # Step 1: Collect sensor data
        temperature, humidity = read_temperature_and_humidity()
        air_quality = read_air_quality()
        
        # Step 2: Send data to ThingSpeak
        send_to_thingspeak(temperature, humidity, air_quality)
        
        # Step 3: Wait before the next reading (every 60 seconds)
        time.sleep(60)  # Collect data every minute

# Main Execution
if __name__ == '__main__':
    logging.info('Environmental Monitoring System started.')
    try:
        # Start the data collection and sending process
        collect_and_send_data()
    except KeyboardInterrupt:
        logging.info('Environmental Monitoring System stopped by user.')
    except Exception as e:
        logging.error(f'Unexpected error: {e}')