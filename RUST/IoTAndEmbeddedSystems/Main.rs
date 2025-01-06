mod sensors;
mod encryption;
mod cloud;

use sensors::generate_sensor_data;
use cloud::send_to_cloud;
use std::time::Duration;
use tokio::time::sleep;

#[tokio::main]
async fn main() {
    println!("Starting IoT Device Firmware...");

    loop {
        // Step 1: Collect sensor data
        let sensor_data = generate_sensor_data();
        println!("Collected Sensor Data: {:?}", sensor_data);

        // Step 2: Send data to the cloud
        match send_to_cloud(&sensor_data).await {
            Ok(_) => println!("Data sent successfully to the cloud."),
            Err(e) => println!("Failed to send data: {}", e),
        }

        // Simulate delay between data transmissions
        sleep(Duration::from_secs(10)).await;
    }
}