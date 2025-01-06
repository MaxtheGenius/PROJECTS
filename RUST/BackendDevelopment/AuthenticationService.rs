#[macro_use] extern crate rocket;

use rocket::{State, serde::json::Json};
use serde::{Serialize, Deserialize};
use rand::Rng;
use uuid::Uuid;
use std::sync::{Arc, Mutex};

#[derive(Serialize, Deserialize, Clone)]
struct User {
    id: String,
    username: String,
    password: String,  // In real systems, use hashed passwords!
    role: String,
}

struct AppState {
    users: Mutex<Vec<User>>,  // Store users in-memory
}

#[post("/register", format = "json", data = "<new_user>")]
async fn register_user(new_user: Json<User>, state: &State<Arc<AppState>>) -> Json<User> {
    let mut users = state.users.lock().unwrap();
    let mut rng = rand::thread_rng();
    let user_id = Uuid::new_v4().to_string();
    let user_role = if rng.gen_bool(0.5) { "admin" } else { "user" };

    let new_user = User {
        id: user_id,
        username: new_user.username.clone(),
        password: new_user.password.clone(),  // Simulate password handling (hashed in real systems)
        role: user_role.to_string(),
    };
    
    users.push(new_user.clone());

    Json(new_user)  // Return the newly registered user
}

#[post("/login", format = "json", data = "<login_data>")]
async fn login_user(login_data: Json<User>, state: &State<Arc<AppState>>) -> Option<Json<User>> {
    let users = state.users.lock().unwrap();
    
    for user in users.iter() {
        if user.username == login_data.username && user.password == login_data.password {
            return Some(Json(user.clone()));  // Return user data if credentials match
        }
    }

    None  // Return None if no match found
}

#[launch]
fn rocket() -> _ {
    let state = Arc::new(AppState {
        users: Mutex::new(vec![]),  // Start with an empty user list
    });

    rocket::build()
        .manage(state)
        .mount("/api/auth", routes![register_user, login_user])
}