use serde::{Deserialize, Serialize};
use chrono::{Utc, DateTime};

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct Content {
    pub id: String,
    pub title: String,
    pub body: String,
    pub author: String,
    pub created_at: DateTime<Utc>,
}