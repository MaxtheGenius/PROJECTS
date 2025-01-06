#[macro_use] extern crate rocket;

use rocket::{State, serde::json::Json};
use serde::{Serialize, Deserialize};
use std::sync::{Arc, Mutex};

#[derive(Serialize, Deserialize, Clone)]
struct Role {
    user_id: String,
    role: String,  // e.g., "admin" or "user"
}

struct AppState {
    roles: Mutex<Vec<Role>>,  // Store roles in-memory
}

#[post("/assign_role", format = "json", data = "<role_assignment>")]
async fn assign_role(role_assignment: Json<Role>, state: &State<Arc<AppState>>) -> Json<Role> {
    let mut roles = state.roles.lock().unwrap();
    roles.push(role_assignment.into_inner());
    Json(role_assignment.into_inner())
}

#[get("/get_role/<user_id>")]
async fn get_role(user_id: String, state: &State<Arc<AppState>>) -> Option<Json<Role>> {
    let roles = state.roles.lock().unwrap();
    for role in roles.iter() {
        if role.user_id == user_id {
            return Some(Json(role.clone()));
        }
    }

    None  // If no role found
}

#[launch]
fn rocket() -> _ {
    let state = Arc::new(AppState {
        roles: Mutex::new(vec![]),  // Initialize roles (empty)
    });

    rocket::build()
        .manage(state)
        .mount("/api/role", routes![assign_role, get_role])
}