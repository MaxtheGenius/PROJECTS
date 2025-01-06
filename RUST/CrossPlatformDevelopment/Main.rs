use rand::Rng;
use serde::Serialize;
use tauri::Manager;

#[derive(Serialize)]
struct Task {
    id: usize,
    title: String,
    description: String,
    completed: bool,
}

#[derive(Serialize)]
struct User {
    id: usize,
    name: String,
    email: String,
}

#[tauri::command]
fn generate_tasks() -> Vec<Task> {
    let titles = ["Plan meeting", "Write report", "Fix bugs", "Prepare slides", "Update docs"];
    let descriptions = [
        "Complete the project proposal.",
        "Review team performance.",
        "Resolve pending issues.",
        "Draft the presentation.",
        "Document API changes.",
    ];

    (1..=10)
        .map(|id| Task {
            id,
            title: titles[rand::thread_rng().gen_range(0..titles.len())].to_string(),
            description: descriptions[rand::thread_rng().gen_range(0..descriptions.len())]
                .to_string(),
            completed: rand::thread_rng().gen_bool(0.5),
        })
        .collect()
}

#[tauri::command]
fn generate_users() -> Vec<User> {
    let names = ["Alice", "Bob", "Charlie", "Dave", "Eve"];
    let domains = ["example.com", "test.org", "demo.net"];

    (1..=5)
        .map(|id| User {
            id,
            name: names[rand::thread_rng().gen_range(0..names.len())].to_string(),
            email: format!(
                "{}@{}",
                names[rand::thread_rng().gen_range(0..names.len())].to_lowercase(),
                domains[rand::thread_rng().gen_range(0..domains.len())]
            ),
        })
        .collect()
}

fn main() {
    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![generate_tasks, generate_users])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}