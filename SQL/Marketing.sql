-- Write a SQL query to segment customers based on their purchase history and browsing behavior

-- Step 1: Create necessary tables for customers, purchases, and browsing history

-- Table to store customer information
CREATE TABLE Customers (
    customer_id INT AUTO_INCREMENT PRIMARY KEY,      -- Unique customer ID
    customer_name VARCHAR(100) NOT NULL,             -- Name of the customer
    email VARCHAR(100) NOT NULL UNIQUE,              -- Customer's email (unique)
    registration_date DATE NOT NULL                  -- Date when the customer registered
);

-- Table to store product categories
CREATE TABLE Categories (
    category_id INT AUTO_INCREMENT PRIMARY KEY,      -- Unique category ID
    category_name VARCHAR(100) NOT NULL              -- Name of the category
);

-- Table to store product information
CREATE TABLE Products (
    product_id INT AUTO_INCREMENT PRIMARY KEY,       -- Unique product ID
    product_name VARCHAR(100) NOT NULL,              -- Product name
    category_id INT,                                -- Foreign key to Categories table
    price DECIMAL(10, 2) NOT NULL,                   -- Price of the product
    stock_quantity INT NOT NULL,                     -- Stock quantity
    FOREIGN KEY (category_id) REFERENCES Categories(category_id)  -- Foreign key to Categories table
);

-- Table to store customer purchases
CREATE TABLE Purchases (
    purchase_id INT AUTO_INCREMENT PRIMARY KEY,      -- Unique purchase ID
    customer_id INT,                                 -- Foreign key to Customers table
    product_id INT,                                  -- Foreign key to Products table
    purchase_date DATE NOT NULL,                     -- Date of purchase
    quantity INT NOT NULL,                           -- Quantity purchased
    total_amount DECIMAL(10, 2) NOT NULL,            -- Total amount spent on the purchase
    FOREIGN KEY (customer_id) REFERENCES Customers(customer_id),  -- Foreign key constraint
    FOREIGN KEY (product_id) REFERENCES Products(product_id)  -- Foreign key constraint
);

-- Table to store customer browsing history
CREATE TABLE BrowsingHistory (
    browsing_id INT AUTO_INCREMENT PRIMARY KEY,      -- Unique browsing record ID
    customer_id INT,                                 -- Foreign key to Customers table
    product_id INT,                                  -- Foreign key to Products table
    browse_date DATE NOT NULL,                       -- Date of browsing
    FOREIGN KEY (customer_id) REFERENCES Customers(customer_id),  -- Foreign key constraint
    FOREIGN KEY (product_id) REFERENCES Products(product_id)  -- Foreign key constraint
);

-- Step 2: Insert sample data

-- Insert sample customers
INSERT INTO Customers (customer_name, email, registration_date) 
VALUES 
    ('John Doe', 'johndoe@email.com', '2021-03-15'),
    ('Jane Smith', 'janesmith@email.com', '2020-06-10'),
    ('Alice Johnson', 'alicej@email.com', '2019-08-21');

-- Insert sample categories
INSERT INTO Categories (category_name)
VALUES 
    ('Electronics'), 
    ('Clothing'),
    ('Home Appliances');

-- Insert sample products
INSERT INTO Products (product_name, category_id, price, stock_quantity)
VALUES
    ('Smartphone', 1, 499.99, 150),
    ('Laptop', 1, 999.99, 80),
    ('T-shirt', 2, 19.99, 200),
    ('Sofa', 3, 299.99, 60);

-- Insert sample purchase data
INSERT INTO Purchases (customer_id, product_id, purchase_date, quantity, total_amount)
VALUES
    (1, 1, '2023-01-15', 1, 499.99),
    (2, 2, '2023-02-20', 1, 999.99),
    (3, 3, '2023-03-10', 2, 39.98),
    (1, 2, '2023-04-05', 1, 999.99),
    (2, 3, '2023-05-10', 3, 59.97);

-- Insert sample browsing history data
INSERT INTO BrowsingHistory (customer_id, product_id, browse_date)
VALUES
    (1, 2, '2023-01-10'),
    (2, 1, '2023-02-18'),
    (3, 1, '2023-03-08'),
    (1, 3, '2023-04-02'),
    (2, 3, '2023-05-02');

-- Step 3: SQL Query to segment customers based on their purchase history and browsing behavior

SELECT 
    c.customer_name,                                  -- Customer name
    COUNT(DISTINCT p.purchase_id) AS total_purchases,  -- Total number of purchases made by the customer
    SUM(p.total_amount) AS total_spent,                -- Total amount spent by the customer
    COUNT(DISTINCT b.browsing_id) AS total_browses,    -- Total number of browsing actions by the customer
    COUNT(DISTINCT CASE WHEN p.purchase_id IS NOT NULL THEN p.purchase_id END) AS purchased_items, -- Items that were purchased
    COUNT(DISTINCT CASE WHEN b.browsing_id IS NOT NULL THEN b.browsing_id END) AS browsed_items,  -- Items that were browsed but not purchased
    CASE 
        WHEN COUNT(DISTINCT p.purchase_id) > 3 THEN 'High-value'
        WHEN COUNT(DISTINCT p.purchase_id) BETWEEN 1 AND 3 THEN 'Medium-value'
        ELSE 'Low-value'
    END AS customer_segment  -- Customer segment based on purchase frequency
FROM 
    Customers c
LEFT JOIN 
    Purchases p ON c.customer_id = p.customer_id      -- Join with Purchases to get purchase details
LEFT JOIN 
    BrowsingHistory b ON c.customer_id = b.customer_id -- Join with BrowsingHistory to get browsing details
GROUP BY 
    c.customer_name
ORDER BY 
    total_spent DESC,                                  -- Order by total amount spent (descending)
    total_purchases DESC;                              -- Then by total number of purchases (descending)