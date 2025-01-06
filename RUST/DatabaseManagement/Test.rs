use tokio::runtime::Runtime;

fn generate_random_content() -> Content {
    let mut rng = rand::thread_rng();
    let id = Uuid::new_v4().to_string();
    let title = format!("Random Title {}", rng.gen_range(1..1000));
    let body = format!("Random Body Content {}", rng.gen_range(1..1000));
    let author = format!("Author {}", rng.gen_range(1..100));
    let created_at = Utc::now();
    
    Content {
        id,
        title,
        body,
        author,
        created_at,
    }
}

fn main() {
    let runtime = Runtime::new().unwrap();
    runtime.block_on(async {
        // Set up the database connection
        let db = Arc::new(Mutex::new(Surreal::connect("surrealdb://localhost").await.unwrap()));

        // Create the ContentManager
        let content_manager = ContentManager::new(db.clone());

        // Generate and insert random content
        for _ in 0..10 {
            let random_content = generate_random_content();
            content_manager.create_content(random_content).await.unwrap();
        }

        // Retrieve and display all content
        let all_content = content_manager.get_all_content().await.unwrap();
        for content in all_content {
            println!("{:?}", content);
        }
    });
}