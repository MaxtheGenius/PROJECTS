<?php

// Start the session to manage user sessions
session_start();

// Database connection setup
$host = "localhost";  // Database host
$dbname = "user_system";  // Database name
$username = "root";  // Database username
$password = "";  // Database password

// Connect to the database
try {
    $pdo = new PDO("mysql:host=$host;dbname=$dbname", $username, $password);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch (PDOException $e) {
    die("Database connection failed: " . $e->getMessage());
}

// Create the database schema
function initializeDatabase($pdo) {
    $queries = [
        // Create users table
        "CREATE TABLE IF NOT EXISTS users (
            id INT AUTO_INCREMENT PRIMARY KEY,
            username VARCHAR(50) NOT NULL UNIQUE,
            password_hash VARCHAR(255) NOT NULL,
            email VARCHAR(100) NOT NULL UNIQUE,
            full_name VARCHAR(100) NOT NULL
        )"
    ];
    foreach ($queries as $query) {
        $pdo->exec($query);
    }
}

// Validate user input
function validateInput($data) {
    return htmlspecialchars(stripslashes(trim($data)));
}

// Registration process
if (isset($_POST['register'])) {
    $username = validateInput($_POST['username']);
    $password = validateInput($_POST['password']);
    $email = validateInput($_POST['email']);
    $full_name = validateInput($_POST['full_name']);

    // Server-side validation
    if (empty($username) || empty($password) || empty($email) || empty($full_name)) {
        $error = "All fields are required.";
    } elseif (!filter_var($email, FILTER_VALIDATE_EMAIL)) {
        $error = "Invalid email format.";
    } else {
        // Check if username or email already exists
        $stmt = $pdo->prepare("SELECT id FROM users WHERE username = :username OR email = :email");
        $stmt->execute(['username' => $username, 'email' => $email]);
        if ($stmt->rowCount() > 0) {
            $error = "Username or email already exists.";
        } else {
            // Register the user
            $password_hash = password_hash($password, PASSWORD_DEFAULT);
            $stmt = $pdo->prepare("INSERT INTO users (username, password_hash, email, full_name) VALUES (:username, :password_hash, :email, :full_name)");
            $stmt->execute(['username' => $username, 'password_hash' => $password_hash, 'email' => $email, 'full_name' => $full_name]);
            $success = "Registration successful. You can now log in.";
        }
    }
}

// Login process
if (isset($_POST['login'])) {
    $username = validateInput($_POST['username']);
    $password = validateInput($_POST['password']);

    // Server-side validation
    if (empty($username) || empty($password)) {
        $error = "Username and password are required.";
    } else {
        // Authenticate the user
        $stmt = $pdo->prepare("SELECT * FROM users WHERE username = :username");
        $stmt->execute(['username' => $username]);
        $user = $stmt->fetch(PDO::FETCH_ASSOC);
        if ($user && password_verify($password, $user['password_hash'])) {
            // Set session variables
            $_SESSION['user_id'] = $user['id'];
            $_SESSION['username'] = $user['username'];
            $_SESSION['full_name'] = $user['full_name'];
            header("Location: dashboard.php");
            exit();
        } else {
            $error = "Invalid username or password.";
        }
    }
}

// Logout process
if (isset($_GET['logout'])) {
    session_destroy();
    header("Location: index.php");
    exit();
}

// Initialize database schema
initializeDatabase($pdo);
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>PHP Registration and Login System</title>
</head>
<body>
    <h1>Welcome to the Registration and Login System</h1>
    
    <!-- Registration Form -->
    <h2>Register</h2>
    <form method="POST" action="">
        <input type="text" name="username" placeholder="Username" required>
        <input type="email" name="email" placeholder="Email" required>
        <input type="text" name="full_name" placeholder="Full Name" required>
        <input type="password" name="password" placeholder="Password" required>
        <button type="submit" name="register">Register</button>
    </form>

    <!-- Login Form -->
    <h2>Login</h2>
    <form method="POST" action="">
        <input type="text" name="username" placeholder="Username" required>
        <input type="password" name="password" placeholder="Password" required>
        <button type="submit" name="login">Login</button>
    </form>

    <!-- Display Error or Success Messages -->
    <?php if (isset($error)) { echo "<p style='color:red;'>$error</p>"; } ?>
    <?php if (isset($success)) { echo "<p style='color:green;'>$success</p>"; } ?>
</body>
</html>

<?php
// Dashboard page (dashboard.php)
if (basename($_SERVER['PHP_SELF']) === 'dashboard.php') {
    if (!isset($_SESSION['user_id'])) {
        header("Location: index.php");
        exit();
    }
    echo "<h1>Welcome, " . $_SESSION['full_name'] . "</h1>";
    echo "<p>Your personalized dashboard</p>";
    echo "<a href='?logout=true'>Logout</a>";
}
?>