-- E-commerce Database Schema for Users, Products, Orders, and Reviews

-- Create the 'Users' table to store user information
CREATE TABLE Users (
    user_id INT AUTO_INCREMENT PRIMARY KEY,      -- Unique user ID (auto-incremented)
    first_name VARCHAR(50) NOT NULL,              -- First name of the user
    last_name VARCHAR(50) NOT NULL,               -- Last name of the user
    email VARCHAR(100) NOT NULL UNIQUE,           -- Email address of the user (unique)
    password VARCHAR(255) NOT NULL,               -- Password (hashed)
    phone_number VARCHAR(15),                     -- Optional: Phone number
    shipping_address TEXT,                        -- Shipping address
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP -- Timestamp when the user account was created
);

-- Insert a sample user into the Users table
INSERT INTO Users (first_name, last_name, email, password, phone_number, shipping_address)
VALUES ('John', 'Doe', 'john.doe@example.com', 'hashed_password_123', '123-456-7890', '1234 Elm St, Springfield, IL');

-- Create the 'Products' table to store product information
CREATE TABLE Products (
    product_id INT AUTO_INCREMENT PRIMARY KEY,    -- Unique product ID (auto-incremented)
    name VARCHAR(100) NOT NULL,                    -- Product name
    description TEXT,                              -- Product description
    price DECIMAL(10, 2) NOT NULL,                 -- Product price
    stock_quantity INT NOT NULL,                   -- Quantity of the product in stock
    category VARCHAR(50),                          -- Product category (optional)
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP -- Timestamp when the product was added
);

-- Insert a sample product into the Products table
INSERT INTO Products (name, description, price, stock_quantity, category)
VALUES ('Wireless Mouse', 'A high-quality wireless mouse.', 29.99, 100, 'Electronics');

-- Create the 'Orders' table to store order details
CREATE TABLE Orders (
    order_id INT AUTO_INCREMENT PRIMARY KEY,        -- Unique order ID (auto-incremented)
    user_id INT NOT NULL,                            -- Foreign key: references Users(user_id)
    order_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,  -- Timestamp when the order was placed
    total_amount DECIMAL(10, 2) NOT NULL,            -- Total price of the order
    shipping_address TEXT,                           -- Shipping address (optional if different from user)
    status VARCHAR(50) DEFAULT 'Processing',         -- Order status (Processing, Shipped, Delivered, etc.)
    FOREIGN KEY (user_id) REFERENCES Users(user_id) -- Foreign key constraint
);

-- Insert a sample order into the Orders table
INSERT INTO Orders (user_id, total_amount, shipping_address, status)
VALUES (1, 59.98, '1234 Elm St, Springfield, IL', 'Processing');

-- Create the 'OrderItems' table to store the products in each order
CREATE TABLE OrderItems (
    order_item_id INT AUTO_INCREMENT PRIMARY KEY,   -- Unique order item ID (auto-incremented)
    order_id INT NOT NULL,                           -- Foreign key: references Orders(order_id)
    product_id INT NOT NULL,                         -- Foreign key: references Products(product_id)
    quantity INT NOT NULL,                           -- Quantity of the product in the order
    price DECIMAL(10, 2) NOT NULL,                   -- Price of the product at the time of order
    FOREIGN KEY (order_id) REFERENCES Orders(order_id),  -- Foreign key constraint
    FOREIGN KEY (product_id) REFERENCES Products(product_id) -- Foreign key constraint
);

-- Insert a sample order item into the OrderItems table
INSERT INTO OrderItems (order_id, product_id, quantity, price)
VALUES (1, 1, 2, 29.99);

-- Create the 'Reviews' table to store product reviews
CREATE TABLE Reviews (
    review_id INT AUTO_INCREMENT PRIMARY KEY,     -- Unique review ID (auto-incremented)
    user_id INT NOT NULL,                          -- Foreign key: references Users(user_id)
    product_id INT NOT NULL,                       -- Foreign key: references Products(product_id)
    rating INT NOT NULL CHECK (rating BETWEEN 1 AND 5), -- Rating between 1 and 5
    review_text TEXT,                              -- Optional text review
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- Timestamp when the review was created
    FOREIGN KEY (user_id) REFERENCES Users(user_id), -- Foreign key constraint
    FOREIGN KEY (product_id) REFERENCES Products(product_id) -- Foreign key constraint
);

-- Insert a sample review into the Reviews table
INSERT INTO Reviews (user_id, product_id, rating, review_text)
VALUES (1, 1, 5, 'This is a great mouse! Very comfortable and responsive.');

-- Sample Queries

-- Get all orders for a specific user
SELECT o.order_id, o.order_date, o.total_amount, o.status
FROM Orders o
JOIN Users u ON o.user_id = u.user_id
WHERE u.email = 'john.doe@example.com';

-- Get all reviews for a specific product
SELECT r.rating, r.review_text, r.created_at
FROM Reviews r
JOIN Products p ON r.product_id = p.product_id
WHERE p.name = 'Wireless Mouse';

-- Get total sales for a product
SELECT p.name, SUM(oi.quantity * oi.price) AS total_sales
FROM OrderItems oi
JOIN Products p ON oi.product_id = p.product_id
GROUP BY p.product_id;