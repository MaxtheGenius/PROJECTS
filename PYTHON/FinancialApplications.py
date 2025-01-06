# Trading Algorithm using Historical and Real-Time Data

import yfinance as yf  # Library for fetching historical data
import pandas as pd    # Library for data manipulation
import numpy as np     # Library for numerical calculations
import datetime
import matplotlib.pyplot as plt  # Library for visualization
import time

# Fetch historical stock data for a specific ticker (e.g., 'AAPL' for Apple)
def get_historical_data(ticker, start_date, end_date):
    """
    Fetch historical stock data from Yahoo Finance.
    
    Parameters:
        ticker (str): Stock symbol (e.g., 'AAPL')
        start_date (str): Start date in 'YYYY-MM-DD' format
        end_date (str): End date in 'YYYY-MM-DD' format
    
    Returns:
        pd.DataFrame: Historical stock data
    """
    stock_data = yf.download(ticker, start=start_date, end=end_date)
    return stock_data

# Calculate the moving average for stock prices (Simple Moving Average - SMA)
def calculate_sma(data, window=50):
    """
    Calculate the simple moving average (SMA) of the stock's closing prices.
    
    Parameters:
        data (pd.DataFrame): Stock data (must contain 'Close' column)
        window (int): The window size for the moving average
    
    Returns:
        pd.Series: SMA values
    """
    sma = data['Close'].rolling(window=window).mean()
    return sma

# Generate buy/sell signals based on the moving average crossover strategy
def generate_signals(data, short_window=50, long_window=200):
    """
    Generate buy and sell signals based on moving average crossover.
    
    Parameters:
        data (pd.DataFrame): Stock data (must contain 'Close' column)
        short_window (int): The short window for the moving average
        long_window (int): The long window for the moving average
    
    Returns:
        pd.DataFrame: Data with buy/sell signals
    """
    data['SMA_short'] = calculate_sma(data, short_window)  # Short-term SMA
    data['SMA_long'] = calculate_sma(data, long_window)   # Long-term SMA
    
    # Create signals
    data['Signal'] = 0  # Default is no signal
    data['Signal'][short_window:] = np.where(data['SMA_short'][short_window:] > data['SMA_long'][short_window:], 1, 0)  # Buy signal when short SMA crosses above long SMA
    data['Position'] = data['Signal'].diff()  # Position will be 1 for buy and -1 for sell signals
    
    return data

# Visualize stock data and trading signals
def visualize_data(data):
    """
    Visualize the stock data with buy and sell signals.
    
    Parameters:
        data (pd.DataFrame): Stock data (must contain 'Close' and 'Position' columns)
    """
    plt.figure(figsize=(12, 6))
    plt.plot(data['Close'], label='Stock Price', color='blue', alpha=0.7)
    plt.plot(data['SMA_short'], label='50-Day SMA', color='orange', alpha=0.7)
    plt.plot(data['SMA_long'], label='200-Day SMA', color='red', alpha=0.7)
    
    # Plot buy signals
    plt.plot(data[data['Position'] == 1].index, data['SMA_short'][data['Position'] == 1], '^', markersize=10, color='green', lw=0, label='Buy Signal')
    
    # Plot sell signals
    plt.plot(data[data['Position'] == -1].index, data['SMA_short'][data['Position'] == -1], 'v', markersize=10, color='red', lw=0, label='Sell Signal')
    
    plt.title('Stock Price and Trading Signals')
    plt.legend()
    plt.show()

# Backtest the trading strategy with historical data
def backtest_strategy(data, initial_balance=10000):
    """
    Backtest the trading strategy to simulate trading decisions on historical data.
    
    Parameters:
        data (pd.DataFrame): Stock data with 'Position' column
        initial_balance (float): Initial balance for trading
    
    Returns:
        float: Final portfolio value after backtest
    """
    balance = initial_balance
    stock_quantity = 0
    
    for index, row in data.iterrows():
        # Execute buy signal
        if row['Position'] == 1 and balance >= row['Close']:
            stock_quantity = balance // row['Close']  # Buy as many shares as possible
            balance -= stock_quantity * row['Close']  # Deduct balance
            print(f"Buy {stock_quantity} shares at {row['Close']:.2f} on {index.date()}")
        
        # Execute sell signal
        elif row['Position'] == -1 and stock_quantity > 0:
            balance += stock_quantity * row['Close']  # Sell all shares
            print(f"Sell {stock_quantity} shares at {row['Close']:.2f} on {index.date()}")
            stock_quantity = 0  # Reset stock quantity
    
    # Final portfolio value (cash + value of any remaining stocks)
    final_value = balance + stock_quantity * data.iloc[-1]['Close']
    return final_value

# Real-Time Trading Simulation (Simulated via a Loop with API calls)
def real_time_trading(ticker, interval='1m', live_duration=10):
    """
    Simulate real-time trading using a live ticker and interval.
    
    Parameters:
        ticker (str): Stock symbol (e.g., 'AAPL')
        interval (str): Data retrieval interval (e.g., '1m', '5m')
        live_duration (int): Duration in minutes for the live trading simulation
    
    Simulates real-time stock price data, generates signals, and makes buy/sell decisions.
    """
    for _ in range(live_duration):
        # Fetch real-time stock data
        data = yf.download(ticker, period='1d', interval=interval)
        
        # Generate trading signals
        signals = generate_signals(data)
        
        # Print the latest signal for decision-making
        if signals['Position'].iloc[-1] == 1:
            print(f"Buy signal for {ticker} at {data['Close'].iloc[-1]:.2f}")
        elif signals['Position'].iloc[-1] == -1:
            print(f"Sell signal for {ticker} at {data['Close'].iloc[-1]:.2f}")
        
        # Wait before fetching new data
        time.sleep(60)  # Simulate 1-minute interval for real-time trading

# Main Execution: Historical Analysis, Backtesting, and Real-time Trading
if __name__ == '__main__':
    # Step 1: Fetch historical data for backtesting (e.g., AAPL stock from 2020-01-01 to 2022-01-01)
    historical_data = get_historical_data('AAPL', '2020-01-01', '2022-01-01')
    
    # Step 2: Generate trading signals based on moving average crossover
    signals = generate_signals(historical_data)
    
    # Step 3: Visualize stock data and trading signals
    visualize_data(signals)
    
    # Step 4: Backtest the trading strategy
    final_balance = backtest_strategy(signals)
    print(f"Final portfolio value after backtest: ${final_balance:.2f}")
    
    # Step 5: Run the real-time trading simulation for 10 minutes
    real_time_trading('AAPL', interval='1m', live_duration=10)