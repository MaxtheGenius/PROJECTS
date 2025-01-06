<?php

/* This PHP script implements a two-factor authentication system for a web application, 
including token generation and verification. */

// Step 1: Start a session to manage user state
session_start();

// Step 2: Function to generate a random 6-digit token
function generateToken() {
    return str_pad(rand(0, 999999), 6, '0', STR_PAD_LEFT); // Ensure 6 digits with leading zeros
}

// Step 3: Function to send the token via email (mocked for simplicity)
function sendTokenViaEmail($email, $token) {
    // In a real application, integrate with an email service (e.g., PHPMailer, SendGrid)
    echo "Token sent to $email: $token\n";
}

// Step 4: Function to verify the token
function verifyToken($inputToken) {
    if (isset($_SESSION['auth_token']) && $_SESSION['auth_token'] === $inputToken) {
        return true;
    }
    return false;
}

// Step 5: Handle form submission and logic
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (isset($_POST['email'])) {
        // Step 5.1: Generate and send token
        $email = filter_var($_POST['email'], FILTER_SANITIZE_EMAIL);
        $token = generateToken();
        $_SESSION['auth_token'] = $token; // Store token in session
        $_SESSION['auth_email'] = $email; // Store email in session
        sendTokenViaEmail($email, $token);
        echo "Token has been sent to your email. Please check your inbox.<br>";
    } elseif (isset($_POST['token'])) {
        // Step 5.2: Verify the token
        $inputToken = filter_var($_POST['token'], FILTER_SANITIZE_STRING);
        if (verifyToken($inputToken)) {
            echo "Authentication successful! You are now logged in.<br>";
            unset($_SESSION['auth_token']); // Clear token after successful login
        } else {
            echo "Invalid token. Please try again.<br>";
        }
    }
}

?>