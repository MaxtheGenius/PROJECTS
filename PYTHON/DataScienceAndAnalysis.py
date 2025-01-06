import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Step 1: Load the sales dataset
# Assuming the dataset is in CSV format with columns: 'date', 'category', 'product_name', 'quantity_sold', 'total_sales'
def load_sales_data(file_path):
    """
    Load the sales dataset from a CSV file.
    """
    try:
        df = pd.read_csv(file_path)
        print("Sales dataset loaded successfully.")
        return df
    except FileNotFoundError:
        print(f"Error: The file at {file_path} was not found.")
        return None

# Step 2: Preprocess and clean the data
def preprocess_data(df):
    """
    Preprocess the dataset by converting 'date' to datetime format and handling missing values.
    """
    df['date'] = pd.to_datetime(df['date'], errors='coerce')  # Convert to datetime, invalid dates will be NaT
    df.dropna(inplace=True)  # Drop rows with NaT or NaN values
    return df

# Step 3: Analyze the dataset
def analyze_sales_data(df):
    """
    Analyze the sales data to find trends such as total sales per product and total quantity sold.
    """
    # Group by product and calculate total sales and quantity sold
    product_sales = df.groupby('product_name').agg(
        total_sales=('total_sales', 'sum'),
        total_quantity_sold=('quantity_sold', 'sum')
    ).reset_index()

    # Sort by total sales in descending order
    product_sales_sorted = product_sales.sort_values(by='total_sales', ascending=False)
    print("\nProduct sales analysis:")
    print(product_sales_sorted)
    
    return product_sales_sorted

# Step 4: Generate trend visualizations
def generate_trend_visualizations(df):
    """
    Generate and display sales trends over time and by product category.
    """
    # Total sales per month
    df['month'] = df['date'].dt.to_period('M')  # Extract month
    monthly_sales = df.groupby('month')['total_sales'].sum().reset_index()

    # Plot total sales trend over time
    plt.figure(figsize=(10, 6))
    plt.plot(monthly_sales['month'].astype(str), monthly_sales['total_sales'], marker='o', color='b')
    plt.title('Monthly Sales Trend')
    plt.xlabel('Month')
    plt.ylabel('Total Sales')
    plt.xticks(rotation=45)
    plt.grid(True)
    plt.tight_layout()
    plt.show()

    # Total sales per category
    category_sales = df.groupby('category')['total_sales'].sum().reset_index()

    # Plot total sales by category
    plt.figure(figsize=(10, 6))
    plt.bar(category_sales['category'], category_sales['total_sales'], color='g')
    plt.title('Total Sales by Product Category')
    plt.xlabel('Product Category')
    plt.ylabel('Total Sales')
    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.show()

# Step 5: Generate an automated report
def generate_report(df, product_sales_sorted):
    """
    Generate an automated sales report.
    """
    report = "Sales Analysis Report\n"
    report += "="*30 + "\n"
    
    # General statistics
    total_sales = df['total_sales'].sum()
    total_quantity_sold = df['quantity_sold'].sum()
    
    report += f"Total Sales: ${total_sales:,.2f}\n"
    report += f"Total Quantity Sold: {total_quantity_sold}\n"
    
    report += "\nTop 5 Products by Sales:\n"
    report += "="*30 + "\n"
    
    # Top 5 products by sales
    top_5_products = product_sales_sorted.head(5)
    for index, row in top_5_products.iterrows():
        report += f"{row['product_name']} - ${row['total_sales']:,.2f} (Total Quantity Sold: {row['total_quantity_sold']})\n"
    
    # Save the report to a file
    with open("sales_report.txt", "w") as f:
        f.write(report)
    
    print("\nSales report generated successfully: 'sales_report.txt'")

# Step 6: Main function to execute the process
def main():
    # Define file path (this should be updated to where the CSV is located)
    file_path = 'sales_data.csv'  # Example file path, replace with actual path to the CSV

    # Load the sales dataset
    df = load_sales_data(file_path)

    if df is not None:
        # Preprocess the data
        df = preprocess_data(df)

        # Analyze the sales data
        product_sales_sorted = analyze_sales_data(df)

        # Generate trend visualizations
        generate_trend_visualizations(df)

        # Generate an automated report
        generate_report(df, product_sales_sorted)

if __name__ == "__main__":
    main()