use surrealdb::sql::{Object, Value};
use surrealdb::Surreal;
use std::sync::Arc;
use tokio::sync::Mutex;
use rand::Rng;
use uuid::Uuid;

pub struct ContentManager {
    db: Arc<Mutex<Surreal>>,
}

impl ContentManager {
    pub fn new(db: Arc<Mutex<Surreal>>) -> Self {
        ContentManager { db }
    }

    pub async fn create_content(&self, content: Content) -> Result<(), Box<dyn std::error::Error>> {
        let mut db = self.db.lock().await;
        db.create("content")
            .content(&content)
            .await?;
        Ok(())
    }

    pub async fn get_content_by_id(&self, content_id: &str) -> Result<Option<Content>, Box<dyn std::error::Error>> {
        let mut db = self.db.lock().await;
        let result = db.query("SELECT * FROM content WHERE id = $id")
            .bind(("id", content_id))
            .await?;
        
        if let Some(record) = result.first() {
            let content: Content = record.into();
            Ok(Some(content))
        } else {
            Ok(None)
        }
    }

    pub async fn get_all_content(&self) -> Result<Vec<Content>, Box<dyn std::error::Error>> {
        let mut db = self.db.lock().await;
        let result = db.query("SELECT * FROM content").await?;
        
        let content_entries = result.into_iter().map(|record: Object| {
            let id = record.get("id").unwrap().to_string();
            let title = record.get("title").unwrap().to_string();
            let body = record.get("body").unwrap().to_string();
            let author = record.get("author").unwrap().to_string();
            let created_at: DateTime<Utc> = record.get("created_at").unwrap().into();
            Content { id, title, body, author, created_at }
        }).collect();
        
        Ok(content_entries)
    }
}