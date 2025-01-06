-- Optimize a complex SQL query causing performance issues in a high-concurrency application

-- Step 1: Create necessary tables for sales, products, and categories (Optimized Version)

-- Table to store product categories
CREATE TABLE Categories (
    category_id INT AUTO_INCREMENT PRIMARY KEY,       -- Unique category ID
    category_name VARCHAR(100) NOT NULL               -- Name of the category
);

-- Table to store products
CREATE TABLE Products (
    product_id INT AUTO_INCREMENT PRIMARY KEY,       -- Unique product ID
    product_name VARCHAR(100) NOT NULL,               -- Name of the product
    category_id INT,                                 -- Foreign key to Categories table
    price DECIMAL(10, 2) NOT NULL,                    -- Price of the product
    stock_quantity INT NOT NULL,                      -- Stock quantity of the product
    FOREIGN KEY (category_id) REFERENCES Categories(category_id)  -- Foreign key constraint
);

-- Table to store sales transactions
CREATE TABLE Sales (
    sale_id INT AUTO_INCREMENT PRIMARY KEY,          -- Unique sale ID
    product_id INT,                                  -- Foreign key to Products table
    sale_date DATE NOT NULL,                          -- Date of the sale
    quantity INT NOT NULL,                            -- Quantity of products sold
    total_amount DECIMAL(10, 2) NOT NULL,             -- Total sale amount (quantity * price)
    FOREIGN KEY (product_id) REFERENCES Products(product_id)  -- Foreign key constraint
);

-- Step 2: Insert sample categories (with some index optimization)
INSERT INTO Categories (category_name) VALUES ('Electronics'), ('Furniture'), ('Clothing');

-- Step 3: Insert sample products (considering appropriate indexing for product searches)
INSERT INTO Products (product_name, category_id, price, stock_quantity)
VALUES 
    ('Smartphone', 1, 499.99, 150),
    ('Laptop', 1, 999.99, 80),
    ('Sofa', 2, 299.99, 60),
    ('T-shirt', 3, 19.99, 200);

-- Step 4: Insert sample sales data (Optimize data insertion and querying using indexing)
INSERT INTO Sales (product_id, sale_date, quantity, total_amount)
VALUES 
    (1, '2022-01-15', 2, 999.98),
    (2, '2022-02-20', 1, 999.99),
    (3, '2022-03-10', 3, 899.97),
    (4, '2022-04-05', 5, 99.95),
    (1, '2022-05-15', 1, 499.99),
    (2, '2022-06-20', 2, 1999.98),
    (3, '2022-07-30', 1, 299.99),
    (4, '2022-08-10', 3, 59.97),
    (1, '2023-01-12', 4, 1999.96),
    (2, '2023-02-28', 2, 1999.98);

-- Step 5: Create necessary indexes to optimize query performance (important for high-concurrency systems)

-- Index on sale_date for faster filtering of sales data by date range
CREATE INDEX idx_sale_date ON Sales(sale_date);

-- Index on category_id in Products for fast lookup of product categories
CREATE INDEX idx_category_id ON Products(category_id);

-- Index on product_id in Sales for faster joins between Products and Sales
CREATE INDEX idx_product_id ON Sales(product_id);

-- Step 6: Optimized complex query to analyze monthly sales trends by product category over the past 3 years

-- The query is optimized for performance by indexing columns used in WHERE, JOIN, and GROUP BY clauses
SELECT 
    c.category_name,                                   -- Product category name
    DATE_FORMAT(s.sale_date, '%Y-%m') AS month,         -- Extract month and year (formatted as YYYY-MM)
    SUM(s.quantity) AS total_quantity_sold,             -- Total quantity sold in the month
    SUM(s.total_amount) AS total_sales,                 -- Total sales amount in the month
    AVG(s.total_amount) AS average_sale_value,          -- Average sale amount per transaction in the month
    COUNT(DISTINCT s.sale_id) AS total_transactions     -- Total number of transactions in the month
FROM 
    Sales s
JOIN 
    Products p ON s.product_id = p.product_id          -- Join Sales and Products on product_id
JOIN 
    Categories c ON p.category_id = c.category_id      -- Join Products and Categories on category_id
WHERE 
    s.sale_date BETWEEN CURDATE() - INTERVAL 3 YEAR AND CURDATE() -- Filter sales data for the last 3 years
GROUP BY 
    c.category_name,                                   -- Group by category
    DATE_FORMAT(s.sale_date, '%Y-%m')                   -- Group by month (YYYY-MM)
ORDER BY 
    month DESC,                                         -- Order by month (descending)
    c.category_name;                                    -- Then by category name

-- Explanation of optimization strategies:
-- 1. Indexing the sale_date, category_id, and product_id columns significantly improves the speed of filtering, 
--    joining, and grouping by these columns.
-- 2. The query is restructured to minimize the use of subqueries and unnecessary operations.
-- 3. Using `COUNT(DISTINCT s.sale_id)` ensures that we get the exact number of transactions without duplicates.