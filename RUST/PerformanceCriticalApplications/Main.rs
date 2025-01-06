mod market;
mod engine;
mod utils;

use crate::engine::TradingEngine;
use crate::market::MarketSimulator;
use crate::utils::generate_random_trading_scenarios;
use tokio::sync::mpsc;

#[tokio::main]
async fn main() {
    // Initialize trading engine and market simulator
    let (order_tx, order_rx) = mpsc::channel(100); // Channel for order processing
    let (trade_tx, trade_rx) = mpsc::channel(100); // Channel for trade reporting

    let mut market = MarketSimulator::new(trade_tx.clone());
    let mut engine = TradingEngine::new(order_rx, trade_rx);

    // Simulate market and trading
    tokio::spawn(async move {
        market.start_simulation(1000).await; // Simulate 1000 random ticks
    });

    tokio::spawn(async move {
        engine.start_engine().await;
    });

    // Generate and submit random trading scenarios
    let scenarios = generate_random_trading_scenarios(500);
    for scenario in scenarios {
        order_tx.send(scenario).await.unwrap();
    }
}