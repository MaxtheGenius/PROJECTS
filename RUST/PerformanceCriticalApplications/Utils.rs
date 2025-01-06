use rand::Rng;
use crate::engine::Order;

pub fn generate_random_trading_scenarios(count: usize) -> Vec<Order> {
    let symbols = vec!["AAPL", "GOOGL", "AMZN", "MSFT", "TSLA"];
    let mut rng = rand::thread_rng();
    let mut scenarios = Vec::new();

    for _ in 0..count {
        scenarios.push(Order {
            symbol: symbols[rng.gen_range(0..symbols.len())].to_string(),
            quantity: rng.gen_range(1..100),
            order_type: if rng.gen_bool(0.5) { "Buy".to_string() } else { "Sell".to_string() },
            price: rng.gen_range(100.0..2000.0),
        });
    }

    scenarios
}