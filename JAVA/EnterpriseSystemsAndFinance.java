import java.util.Random;

/*
 * AlgorithmicTradingSystem.java
 * Implements a basic algorithmic trading system in Java that analyzes real-time market data and executes trades automatically.
 */
public class AlgorithmicTradingSystem {

    // Simulates the state of the market in real-time (e.g., the price of a stock)
    private static class MarketData {
        private double price;

        public MarketData() {
            this.price = 100.0; // Initial price
        }

        // Simulates the market price update
        public void updatePrice() {
            Random rand = new Random();
            this.price += rand.nextDouble() * 2 - 1; // Random changes between -1 and +1
        }

        // Gets the current market price
        public double getPrice() {
            return this.price;
        }
    }

    // Simulates a user account to buy and sell
    private static class TradingAccount {
        private double balance;
        private int shares;

        public TradingAccount(double initialBalance) {
            this.balance = initialBalance;
            this.shares = 0;
        }

        // Checks if there are enough funds to buy shares
        public boolean canBuy(double price, int quantity) {
            return balance >= price * quantity;
        }

        // Executes a buy order
        public void buy(double price, int quantity) {
            if (canBuy(price, quantity)) {
                balance -= price * quantity;
                shares += quantity;
                System.out.println("Bought " + quantity + " shares at price " + price);
            } else {
                System.out.println("Insufficient funds to buy shares.");
            }
        }

        // Executes a sell order
        public void sell(double price, int quantity) {
            if (shares >= quantity) {
                balance += price * quantity;
                shares -= quantity;
                System.out.println("Sold " + quantity + " shares at price " + price);
            } else {
                System.out.println("Not enough shares to sell.");
            }
        }

        // Displays the balance and shares
        public void displayAccount() {
            System.out.println("Balance: $" + balance + ", Shares: " + shares);
        }
    }

    // Simple trading algorithm that buys if the price is low and sells if the price
    // is high
    private static class TradingAlgorithm {
        private static final double BUY_THRESHOLD = 99.0; // Threshold to buy
        private static final double SELL_THRESHOLD = 101.0; // Threshold to sell

        private TradingAccount account;
        private MarketData marketData;

        public TradingAlgorithm(TradingAccount account, MarketData marketData) {
            this.account = account;
            this.marketData = marketData;
        }

        // Executes the trade based on market conditions
        public void executeTrade() {
            double price = marketData.getPrice();
            System.out.println("Market price: " + price);

            // If the price is low, buy shares
            if (price < BUY_THRESHOLD) {
                int quantity = 10; // Number of shares to buy
                if (account.canBuy(price, quantity)) {
                    account.buy(price, quantity);
                }
            }
            // If the price is high, sell shares
            else if (price > SELL_THRESHOLD) {
                int quantity = 10; // Number of shares to sell
                account.sell(price, quantity);
            }

            account.displayAccount();
        }
    }

    // Main to simulate the trading system
    public static void main(String[] args) {
        // Create market data and trading account
        MarketData marketData = new MarketData();
        TradingAccount account = new TradingAccount(1000.0); // Initial balance of $1000

        // Create and execute the trading algorithm
        TradingAlgorithm algorithm = new TradingAlgorithm(account, marketData);

        // Run the trading system continuously
        for (int i = 0; i < 20; i++) {
            marketData.updatePrice(); // Simulate the market price change
            algorithm.executeTrade(); // Execute the trade
            try {
                Thread.sleep(1000); // Wait 1 second before the next iteration
            } catch (InterruptedException e) {
                System.out.println("Thread interrupted.");
            }
        }
    }
}