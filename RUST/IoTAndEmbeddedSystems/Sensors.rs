use rand::Rng;
use serde::Serialize;

#[derive(Debug, Serialize)]
pub struct SensorData {
    temperature: f32, // in Celsius
    humidity: f32,    // percentage
    air_quality: u32, // AQI (Air Quality Index)
}

// Simulates sensor data
pub fn generate_sensor_data() -> SensorData {
    let mut rng = rand::thread_rng();

    SensorData {
        temperature: rng.gen_range(-10.0..40.0), // -10°C to 40°C
        humidity: rng.gen_range(20.0..100.0),    // 20% to 100%
        air_quality: rng.gen_range(0..500),     // AQI from 0 to 500
    }
}