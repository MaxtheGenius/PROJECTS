<?php

// RESTful API for Inventory Management System

// Set the content type to JSON and allow cross-origin requests
header("Content-Type: application/json");
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: GET, POST, PUT, DELETE");
header("Access-Control-Allow-Headers: Content-Type, Access-Control-Allow-Headers, Authorization, X-Requested-With");

// Database connection parameters
$host = "localhost";
$db_name = "APIDevelopment"; // Updated database name
$username = "root";
$password = "";

// Establish database connection
try {
    $conn = new PDO("mysql:host=$host;dbname=$db_name", $username, $password);
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch (PDOException $e) {
    echo json_encode(["error" => "Database connection failed: " . $e->getMessage()]);
    exit();
}

// Helper function to send JSON responses
function sendResponse($status, $data) {
    http_response_code($status);
    echo json_encode($data);
    exit();
}

// Parse the request method and endpoint
$request_method = $_SERVER['REQUEST_METHOD'];
$request_uri = explode("/", trim($_SERVER['PATH_INFO'], "/"));

// Determine the resource being accessed
$resource = $request_uri[0] ?? null;
$id = $request_uri[1] ?? null;

// Handle the API requests
switch ($resource) {
    case "items":
        handleItems($request_method, $id, $conn);
        break;
    default:
        sendResponse(404, ["error" => "Resource not found."]);
}

// Function to handle requests related to inventory items
function handleItems($method, $id, $conn) {
    switch ($method) {
        case "GET":
            if ($id) {
                // Fetch a single item
                $stmt = $conn->prepare("SELECT * FROM items WHERE id = ?");
                $stmt->execute([$id]);
                $item = $stmt->fetch(PDO::FETCH_ASSOC);
                if ($item) {
                    sendResponse(200, $item);
                } else {
                    sendResponse(404, ["error" => "Item not found."]);
                }
            } else {
                // Fetch all items
                $stmt = $conn->query("SELECT * FROM items");
                $items = $stmt->fetchAll(PDO::FETCH_ASSOC);
                sendResponse(200, $items);
            }
            break;

        case "POST":
            // Add a new item
            $data = json_decode(file_get_contents("php://input"), true);
            if (isset($data['name'], $data['quantity'], $data['price'])) {
                $stmt = $conn->prepare("INSERT INTO items (name, quantity, price) VALUES (?, ?, ?)");
                $stmt->execute([$data['name'], $data['quantity'], $data['price']]);
                sendResponse(201, ["message" => "Item created successfully."]);
            } else {
                sendResponse(400, ["error" => "Invalid input data."]);
            }
            break;

        case "PUT":
            // Update an existing item
            if ($id) {
                $data = json_decode(file_get_contents("php://input"), true);
                if (isset($data['name'], $data['quantity'], $data['price'])) {
                    $stmt = $conn->prepare("UPDATE items SET name = ?, quantity = ?, price = ? WHERE id = ?");
                    $stmt->execute([$data['name'], $data['quantity'], $data['price'], $id]);
                    sendResponse(200, ["message" => "Item updated successfully."]);
                } else {
                    sendResponse(400, ["error" => "Invalid input data."]);
                }
            } else {
                sendResponse(400, ["error" => "Item ID is required for update."]);
            }
            break;

        case "DELETE":
            // Delete an item
            if ($id) {
                $stmt = $conn->prepare("DELETE FROM items WHERE id = ?");
                $stmt->execute([$id]);
                sendResponse(200, ["message" => "Item deleted successfully."]);
            } else {
                sendResponse(400, ["error" => "Item ID is required for deletion."]);
            }
            break;

        default:
            sendResponse(405, ["error" => "Method not allowed."]);
    }
}

?>