mod scraper;
mod test_data;

use scraper::{scrape_page, ScrapeResult};
use test_data::generate_test_urls;
use tokio::sync::Semaphore;
use std::sync::Arc;

#[tokio::main]
async fn main() {
    // Configuration
    const CONCURRENT_REQUESTS: usize = 10; // Maximum concurrent requests
    const TEST_URL_COUNT: usize = 50;     // Number of test URLs

    // Generate test URLs
    let urls = generate_test_urls(TEST_URL_COUNT);

    // Shared semaphore to respect concurrency limits
    let semaphore = Arc::new(Semaphore::new(CONCURRENT_REQUESTS));

    // Concurrently process all URLs
    let mut tasks = vec![];
    for url in urls {
        let permit = semaphore.clone().acquire_owned().await.unwrap();
        tasks.push(tokio::spawn(async move {
            let result = scrape_page(&url).await;
            drop(permit); // Release semaphore permit
            (url, result)
        }));
    }

    // Collect and display results
    for task in tasks {
        match task.await {
            Ok((url, Ok(data))) => {
                println!("SUCCESS: Scraped {}: {}", url, data.summary());
            }
            Ok((url, Err(err))) => {
                println!("ERROR: Failed to scrape {}: {}", url, err);
            }
            Err(err) => {
                println!("CRITICAL: Task panicked: {}", err);
            }
        }
    }
}