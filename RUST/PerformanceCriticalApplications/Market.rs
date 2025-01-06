use rand::Rng;
use tokio::sync::mpsc::Sender;

#[derive(Debug, Clone)]
pub struct MarketData {
    pub symbol: String,
    pub bid_price: f64,
    pub ask_price: f64,
    pub timestamp: i64,
}

pub struct MarketSimulator {
    trade_tx: Sender<MarketData>,
}

impl MarketSimulator {
    pub fn new(trade_tx: Sender<MarketData>) -> Self {
        Self { trade_tx }
    }

    pub async fn start_simulation(&mut self, tick_count: usize) {
        let symbols = vec!["AAPL", "GOOGL", "AMZN", "MSFT", "TSLA"];
        let mut rng = rand::thread_rng();

        for _ in 0..tick_count {
            let data = MarketData {
                symbol: symbols[rng.gen_range(0..symbols.len())].to_string(),
                bid_price: rng.gen_range(100.0..1000.0),
                ask_price: rng.gen_range(1000.0..2000.0),
                timestamp: chrono::Utc::now().timestamp_nanos(),
            };

            self.trade_tx.send(data).await.unwrap();
            tokio::time::sleep(tokio::time::Duration::from_millis(10)).await;
        }
    }
}