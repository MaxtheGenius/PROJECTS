use reqwest::Error as ReqwestError;
use serde::Deserialize;

#[derive(Debug, Deserialize)]
pub struct ScrapeResult {
    pub title: String,
    pub word_count: usize,
}

impl ScrapeResult {
    // Summary of the scrape result
    pub fn summary(&self) -> String {
        format!("Title: '{}', Words: {}", self.title, self.word_count)
    }
}

// Scrapes a single web page
pub async fn scrape_page(url: &str) -> Result<ScrapeResult, String> {
    // Make HTTP GET request
    let response = reqwest::get(url).await.map_err(|e| format!("Request error: {}", e))?;

    // Check for non-200 status codes
    if !response.status().is_success() {
        return Err(format!("HTTP error: {}", response.status()));
    }

    // Extract HTML content
    let html = response.text().await.map_err(|e| format!("Body read error: {}", e))?;

    // Simulate HTML parsing (extract title and count words)
    let title = extract_title(&html).unwrap_or_else(|| "Untitled".to_string());
    let word_count = count_words(&html);

    Ok(ScrapeResult { title, word_count })
}

// Extracts the title of an HTML page
fn extract_title(html: &str) -> Option<String> {
    html.split("<title>")
        .nth(1)?
        .split("</title>")
        .next()
        .map(|s| s.trim().to_string())
}

// Counts the number of words in the HTML content
fn count_words(html: &str) -> usize {
    html.split_whitespace().count()
}