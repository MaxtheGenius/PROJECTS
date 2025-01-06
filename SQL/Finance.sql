-- Create a SQL report that calculates quarterly revenue projections based on historical data and current trends

-- Step 1: Create necessary tables for products, sales, and trends

-- Table to store product details
CREATE TABLE Products (
    product_id INT AUTO_INCREMENT PRIMARY KEY,       -- Unique product ID
    product_name VARCHAR(100) NOT NULL,               -- Name of the product
    price DECIMAL(10, 2) NOT NULL                     -- Price of the product
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

-- Table to store trend data (growth rate per product)
CREATE TABLE Trends (
    product_id INT,                                  -- Foreign key to Products table
    growth_rate DECIMAL(5, 2) NOT NULL,               -- Growth rate (percentage) for revenue projection
    FOREIGN KEY (product_id) REFERENCES Products(product_id)  -- Foreign key constraint
);

-- Step 2: Insert sample products
INSERT INTO Products (product_name, price)
VALUES 
    ('Smartphone', 499.99),
    ('Laptop', 999.99),
    ('Sofa', 299.99),
    ('T-shirt', 19.99);

-- Step 3: Insert sample sales data
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

-- Step 4: Insert trend data (example growth rates)
INSERT INTO Trends (product_id, growth_rate)
VALUES 
    (1, 5.00),  -- 5% growth rate for Smartphones
    (2, 4.00),  -- 4% growth rate for Laptops
    (3, 3.00),  -- 3% growth rate for Sofas
    (4, 2.50);  -- 2.5% growth rate for T-shirts

-- Step 5: Complex Query to calculate quarterly revenue projections

WITH QuarterlySales AS (
    -- Aggregate sales data by product and quarter
    SELECT 
        p.product_name,                                  -- Product name
        YEAR(s.sale_date) AS year,                        -- Extract year from sale date
        QUARTER(s.sale_date) AS quarter,                  -- Extract quarter from sale date
        SUM(s.total_amount) AS total_sales               -- Total sales per quarter
    FROM 
        Sales s
    JOIN 
        Products p ON s.product_id = p.product_id       -- Join Sales and Products on product_id
    GROUP BY 
        p.product_name, YEAR(s.sale_date), QUARTER(s.sale_date) -- Group by product, year, and quarter
),
ProjectedRevenue AS (
    -- Calculate projected revenue for the next quarter based on growth rate
    SELECT 
        qs.product_name,                                 -- Product name
        qs.year,                                         -- Year of sales
        qs.quarter,                                      -- Quarter of sales
        qs.total_sales,                                  -- Total sales in the current quarter
        t.growth_rate,                                   -- Growth rate for the product
        (qs.total_sales * (1 + t.growth_rate / 100)) AS projected_sales -- Projected sales for the next quarter
    FROM 
        QuarterlySales qs
    JOIN 
        Trends t ON qs.product_name = t.product_name    -- Join QuarterlySales with Trends on product_name
)
-- Final query to display projected revenue
SELECT 
    product_name,                                      -- Product name
    year,                                              -- Year of sales
    quarter,                                           -- Quarter of sales
    total_sales,                                       -- Total sales in the current quarter
    projected_sales                                    -- Projected sales for the next quarter
FROM 
    ProjectedRevenue
ORDER BY 
    product_name, year, quarter;                       -- Order by product, year, and quarter