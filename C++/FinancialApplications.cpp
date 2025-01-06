#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

// Structure representing market data
struct MarketData {
    string symbol;    // Stock or asset symbol
    double price;     // Current price of the asset
    long timestamp;   // Timestamp of the data (in milliseconds)
};

// Structure representing a trade order
struct TradeOrder {
    string symbol;    // Stock or asset symbol
    string type;      // "BUY" or "SELL"
    int quantity;     // Number of units to trade
    double price;     // Price at which the trade is executed
};

// Class for simulating real-time market data feed
class MarketDataFeed {
private:
    vector<string> symbols;         // List of symbols
    default_random_engine engine;   // Random engine for price generation
    uniform_real_distribution<double> priceDist;

public:
    // Constructor
    MarketDataFeed(const vector<string> &symbols)
        : symbols(symbols), priceDist(50.0, 200.0), engine(static_cast<unsigned>(chrono::system_clock::now().time_since_epoch().count())) {}

    // Generate a single market data update
    MarketData generateMarketData() {
        string symbol = symbols[engine() % symbols.size()];
        double price = priceDist(engine);
        long timestamp = chrono::system_clock::now().time_since_epoch().count();
        return MarketData{symbol, price, timestamp};
    }
};

// Class for managing predefined trading strategies
class TradingStrategy {
private:
    map<string, double> thresholds; // Thresholds for buying and selling

public:
    // Constructor
    TradingStrategy(const map<string, double> &thresholds) : thresholds(thresholds) {}

    // Decide whether to trade based on market data
    vector<TradeOrder> evaluateMarketData(const MarketData &data) {
        vector<TradeOrder> orders;

        // Check if the data matches any predefined strategy
        if (thresholds.find(data.symbol) != thresholds.end()) {
            double threshold = thresholds.at(data.symbol);

            // If price is below threshold, create a BUY order
            if (data.price < threshold * 0.95) {
                orders.push_back(TradeOrder{data.symbol, "BUY", 10, data.price});
            }
            // If price is above threshold, create a SELL order
            else if (data.price > threshold * 1.05) {
                orders.push_back(TradeOrder{data.symbol, "SELL", 10, data.price});
            }
        }

        return orders;
    }
};

// Class for executing trades
class TradeExecutor {
public:
    // Execute a trade order
    void executeTrade(const TradeOrder &order) {
        cout << "Executing trade: " << order.type
             << " " << order.quantity << " units of " << order.symbol
             << " at price " << order.price << "\n";
    }
};

// Main function to simulate the trading system
int main() {
    // Define symbols and thresholds
    vector<string> symbols = {"AAPL", "GOOGL", "AMZN", "MSFT"};
    map<string, double> thresholds = {
        {"AAPL", 150.0},
        {"GOOGL", 2800.0},
        {"AMZN", 3400.0},
        {"MSFT", 300.0}
    };

    // Create instances of feed, strategy, and executor
    MarketDataFeed feed(symbols);
    TradingStrategy strategy(thresholds);
    TradeExecutor executor;

    // Simulate trading system for 10 updates
    for (int i = 0; i < 10; ++i) {
        MarketData data = feed.generateMarketData();
        cout << "Market Data: " << data.symbol << " Price: " << data.price << "\n";

        // Evaluate strategy and execute trades
        vector<TradeOrder> orders = strategy.evaluateMarketData(data);
        for (const auto &order : orders) {
            executor.executeTrade(order);
        }

        cout << "\n";
    }

    return 0;
}