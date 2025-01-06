#[macro_use] extern crate rocket;

use rocket::{State, serde::json::Json};
use serde::{Serialize, Deserialize};
use rand::Rng;
use std::sync::{Arc, Mutex};
use uuid::Uuid;

#[derive(Serialize, Deserialize, Clone)]
struct Profile {
    user_id: String,
    name: String,
    email: String,
    age: u32,
}

struct AppState {
    profiles: Mutex<Vec<Profile>>,  // Store profiles in-memory
}

#[post("/create_profile", format = "json", data = "<profile>")]
async fn create_profile(profile: Json<Profile>, state: &State<Arc<AppState>>) -> Json<Profile> {
    let mut profiles = state.profiles.lock().unwrap();
    let user_profile = Profile {
        user_id: profile.user_id.clone(),
        name: profile.name.clone(),
        email: profile.email.clone(),
        age: rand::thread_rng().gen_range(18..99),  // Random age
    };

    profiles.push(user_profile.clone());
    Json(user_profile)  // Return the created profile
}

#[get("/get_profile/<user_id>")]
async fn get_profile(user_id: String, state: &State<Arc<AppState>>) -> Option<Json<Profile>> {
    let profiles = state.profiles.lock().unwrap();
    for profile in profiles.iter() {
        if profile.user_id == user_id {
            return Some(Json(profile.clone()));
        }
    }

    None  // If no profile found
}

#[launch]
fn rocket() -> _ {
    let state = Arc::new(AppState {
        profiles: Mutex::new(vec![]),  // Start with an empty profile list
    });

    rocket::build()
        .manage(state)
        .mount("/api/profile", routes![create_profile, get_profile])
}