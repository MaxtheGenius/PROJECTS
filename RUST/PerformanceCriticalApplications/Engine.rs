use tokio::sync::mpsc::{Receiver, Sender};
use crate::market::MarketData;

#[derive(Debug)]
pub struct Order {
    pub symbol: String,
    pub quantity: usize,
    pub order_type: String, // "Buy" or "Sell"
    pub price: f64,
}

pub struct TradingEngine {
    order_rx: Receiver<Order>,
    trade_rx: Receiver<MarketData>,
}

impl TradingEngine {
    pub fn new(order_rx: Receiver<Order>, trade_rx: Receiver<MarketData>) -> Self {
        Self { order_rx, trade_rx }
    }

    pub async fn start_engine(&mut self) {
        let mut market_data_cache = Vec::new();

        loop {
            tokio::select! {
                Some(order) = self.order_rx.recv() => {
                    self.process_order(order, &market_data_cache).await;
                }
                Some(trade) = self.trade_rx.recv() => {
                    market_data_cache.push(trade);
                    if market_data_cache.len() > 100 { // Keep cache size manageable
                        market_data_cache.remove(0);
                    }
                }
            }
        }
    }

    async fn process_order(&self, order: Order, market_data_cache: &[MarketData]) {
        // Find matching market data
        if let Some(latest_data) = market_data_cache.iter()
            .filter(|data| data.symbol == order.symbol)
            .last()
        {
            if (order.order_type == "Buy" && order.price >= latest_data.ask_price) ||
               (order.order_type == "Sell" && order.price <= latest_data.bid_price) {
                println!("Order executed: {:?}", order);
            } else {
                println!("Order not executed: {:?}", order);
            }
        } else {
            println!("No market data available for order: {:?}", order);
        }
    }
}