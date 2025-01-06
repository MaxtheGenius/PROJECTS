-- SQL to create the database and table structure

CREATE DATABASE APIDevelopment;

USE APIDevelopment;

CREATE TABLE items (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    quantity INT NOT NULL,
    price DECIMAL(10, 2) NOT NULL
);

INSERT INTO items (name, quantity, price) VALUES
('Laptop', 10, 799.99),
('Mouse', 50, 19.99),
('Keyboard', 30, 49.99);