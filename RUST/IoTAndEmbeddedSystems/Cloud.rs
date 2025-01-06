use crate::encryption::encrypt_data;
use crate::sensors::SensorData;
use reqwest::Client;
use std::time::Duration;

const CLOUD_URL: &str = "https://mock-cloud-service.com/upload";

pub async fn send_to_cloud(sensor_data: &SensorData) -> Result<(), String> {
    // Encrypt sensor data
    let encrypted_data = encrypt_data(sensor_data)?;

    // Send encrypted data to the cloud
    let client = Client::new();
    let response = client
        .post(CLOUD_URL)
        .timeout(Duration::from_secs(5))
        .body(encrypted_data)
        .header("Content-Type", "application/json")
        .send()
        .await
        .map_err(|e| format!("Request error: {}", e))?;

    // Handle response status
    if response.status().is_success() {
        Ok(())
    } else {
        Err(format!("Cloud error: {}", response.status()))
    }
}