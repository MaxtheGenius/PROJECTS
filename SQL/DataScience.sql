-- Implement a series of SQL queries to perform exploratory data analysis on a global weather dataset

-- Step 1: Create necessary tables for weather data, countries, and cities

-- Table to store country information
CREATE TABLE Countries (
    country_id INT AUTO_INCREMENT PRIMARY KEY,         -- Unique country ID
    country_name VARCHAR(100) NOT NULL                 -- Name of the country
);

-- Table to store city information
CREATE TABLE Cities (
    city_id INT AUTO_INCREMENT PRIMARY KEY,           -- Unique city ID
    city_name VARCHAR(100) NOT NULL,                   -- Name of the city
    country_id INT,                                   -- Foreign key to Countries table
    latitude DECIMAL(9, 6) NOT NULL,                   -- Latitude of the city
    longitude DECIMAL(9, 6) NOT NULL,                  -- Longitude of the city
    FOREIGN KEY (country_id) REFERENCES Countries(country_id) -- Foreign key constraint
);

-- Table to store weather records
CREATE TABLE Weather (
    weather_id INT AUTO_INCREMENT PRIMARY KEY,         -- Unique weather record ID
    city_id INT,                                       -- Foreign key to Cities table
    observation_date DATE NOT NULL,                     -- Date of weather observation
    temperature DECIMAL(5, 2) NOT NULL,                 -- Temperature in Celsius
    humidity DECIMAL(5, 2) NOT NULL,                    -- Humidity in percentage
    precipitation DECIMAL(5, 2),                        -- Precipitation in millimeters
    wind_speed DECIMAL(5, 2),                           -- Wind speed in m/s
    FOREIGN KEY (city_id) REFERENCES Cities(city_id)  -- Foreign key constraint
);

-- Step 2: Insert sample countries
INSERT INTO Countries (country_name) VALUES ('United States'), ('India'), ('Germany'), ('Brazil'), ('Australia');

-- Step 3: Insert sample cities with country information
INSERT INTO Cities (city_name, country_id, latitude, longitude)
VALUES 
    ('New York', 1, 40.7128, -74.0060),
    ('Delhi', 2, 28.6139, 77.2090),
    ('Berlin', 3, 52.52, 13.4050),
    ('São Paulo', 4, -23.5505, -46.6333),
    ('Sydney', 5, -33.8688, 151.2093);

-- Step 4: Insert sample weather data for cities
INSERT INTO Weather (city_id, observation_date, temperature, humidity, precipitation, wind_speed)
VALUES 
    (1, '2023-01-01', 5.2, 75, 2.5, 3.5),
    (2, '2023-01-01', 18.7, 60, 1.2, 4.2),
    (3, '2023-01-01', -1.0, 80, 0.0, 2.0),
    (4, '2023-01-01', 25.1, 85, 3.3, 5.1),
    (5, '2023-01-01', 22.5, 70, 0.8, 4.0),
    (1, '2023-01-02', 3.8, 78, 3.0, 3.0),
    (2, '2023-01-02', 19.2, 62, 2.0, 4.5),
    (3, '2023-01-02', 0.3, 85, 0.5, 1.5),
    (4, '2023-01-02', 26.3, 83, 1.1, 4.9),
    (5, '2023-01-02', 23.1, 72, 0.5, 3.8);

-- Step 5: Exploratory Data Analysis (EDA) Queries

-- 5.1: Get the average temperature by country
SELECT 
    c.country_name, 
    AVG(w.temperature) AS avg_temperature
FROM 
    Weather w
JOIN 
    Cities ci ON w.city_id = ci.city_id
JOIN 
    Countries c ON ci.country_id = c.country_id
GROUP BY 
    c.country_name
ORDER BY 
    avg_temperature DESC;

-- 5.2: Get the total precipitation per city
SELECT 
    ci.city_name, 
    SUM(w.precipitation) AS total_precipitation
FROM 
    Weather w
JOIN 
    Cities ci ON w.city_id = ci.city_id
GROUP BY 
    ci.city_name
ORDER BY 
    total_precipitation DESC;

-- 5.3: Get the average humidity per country
SELECT 
    c.country_name, 
    AVG(w.humidity) AS avg_humidity
FROM 
    Weather w
JOIN 
    Cities ci ON w.city_id = ci.city_id
JOIN 
    Countries c ON ci.country_id = c.country_id
GROUP BY 
    c.country_name
ORDER BY 
    avg_humidity DESC;

-- 5.4: Get the city with the highest wind speed
SELECT 
    ci.city_name, 
    MAX(w.wind_speed) AS max_wind_speed
FROM 
    Weather w
JOIN 
    Cities ci ON w.city_id = ci.city_id
GROUP BY 
    ci.city_name
ORDER BY 
    max_wind_speed DESC
LIMIT 1;

-- 5.5: Get the temperature variation between the highest and lowest temperature for each city
SELECT 
    ci.city_name, 
    MAX(w.temperature) - MIN(w.temperature) AS temperature_variation
FROM 
    Weather w
JOIN 
    Cities ci ON w.city_id = ci.city_id
GROUP BY 
    ci.city_name
ORDER BY 
    temperature_variation DESC;

-- 5.6: Get the total number of weather records for each city
SELECT 
    ci.city_name, 
    COUNT(w.weather_id) AS total_records
FROM 
    Weather w
JOIN 
    Cities ci ON w.city_id = ci.city_id
GROUP BY 
    ci.city_name
ORDER BY 
    total_records DESC;