use sysinfo::{System, SystemExt, CpuExt, NetworkExt};
use rand::Rng;

pub fn get_system_metrics() -> SystemMetrics {
    let mut rng = rand::thread_rng();

    // Fetch system information
    let mut sys = System::new_all();
    sys.refresh_all(); // Refresh all information

    // Simulate load with random values
    let cpu_usage = rng.gen_range(10.0..90.0);  // Simulated CPU usage between 10% and 90%
    let memory_usage = rng.gen_range(20.0..80.0);  // Simulated memory usage between 20% and 80%
    let network_usage = rng.gen_range(50.0..100.0);  // Simulated network usage between 50% and 100%

    // Get the current timestamp
    let timestamp = Utc::now();

    SystemMetrics {
        timestamp,
        cpu_usage,
        memory_usage,
        network_usage,
    }
}