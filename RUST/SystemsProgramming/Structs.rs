use serde::{Deserialize, Serialize};
use chrono::{Utc, DateTime};

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct SystemMetrics {
    pub timestamp: DateTime<Utc>,
    pub cpu_usage: f32,
    pub memory_usage: f32,
    pub network_usage: f32,
}