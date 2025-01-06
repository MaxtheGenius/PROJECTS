use rand::seq::SliceRandom;
use rand::Rng;

// Generates a list of random test URLs
pub fn generate_test_urls(count: usize) -> Vec<String> {
    let domains = vec![
        "example.com",
        "testsite.org",
        "demo.net",
        "mockpage.io",
    ];

    let mut urls = vec![];
    let mut rng = rand::thread_rng();

    for _ in 0..count {
        let domain = domains.choose(&mut rng).unwrap();
        let path: String = (0..10)
            .map(|_| (b'a' + (rng.gen::<u8>() % 26)) as char)
            .collect();
        urls.push(format!("https://{}/{}", domain, path));
    }

    urls
}