#[macro_use] extern crate rocket;

use rocket::{State, serde::json::Json};
use serde::{Serialize, Deserialize};
use std::sync::{Arc, Mutex};
use rocket::tokio::sync::broadcast;

#[derive(Serialize, Deserialize, Clone)]
struct Document {
    id: u32,
    title: String,
    content: String,
    last_edited_by: String,
}

struct AppState {
    documents: Mutex<Vec<Document>>,  // Store documents
    tx: broadcast::Sender<String>,    // For real-time updates
}

#[post("/create", format = "json", data = "<document>")]
async fn create_document(document: Json<Document>, state: &State<Arc<AppState>>) -> Json<Document> {
    let mut documents = state.documents.lock().unwrap();
    documents.push(document.into_inner());
    
    // Broadcast to all subscribers about the document update
    let _ = state.tx.send(format!("New document created: {}", document.title));
    
    Json(document.into_inner())  // Return the created document
}

#[get("/documents")]
fn get_documents(state: &State<Arc<AppState>>) -> Json<Vec<Document>> {
    let documents = state.documents.lock().unwrap();
    Json(documents.clone())  // Return all documents
}

#[launch]
fn rocket() -> _ {
    let (tx, _) = broadcast::channel::<String>(100);
    let state = Arc::new(AppState {
        documents: Mutex::new(vec![]),  // Initialize with empty documents
        tx,
    });
    
    rocket::build()
        .manage(state)
        .mount("/api", routes![create_document, get_documents])
}