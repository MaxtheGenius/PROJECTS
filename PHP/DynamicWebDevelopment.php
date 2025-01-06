<?php

// Database connection
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "cms";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Create necessary tables if they do not exist

// Table to store categories
$query = "CREATE TABLE IF NOT EXISTS Categories (
    category_id INT AUTO_INCREMENT PRIMARY KEY,
    category_name VARCHAR(100) NOT NULL
)";
$conn->query($query);

// Table to store tags
$query = "CREATE TABLE IF NOT EXISTS Tags (
    tag_id INT AUTO_INCREMENT PRIMARY KEY,
    tag_name VARCHAR(100) NOT NULL
)";
$conn->query($query);

// Table to store users (for simplicity, this can be expanded with authentication methods)
$query = "CREATE TABLE IF NOT EXISTS Users (
    user_id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(100) NOT NULL,
    password VARCHAR(255) NOT NULL
)";
$conn->query($query);

// Table to store articles
$query = "CREATE TABLE IF NOT EXISTS Articles (
    article_id INT AUTO_INCREMENT PRIMARY KEY,
    title VARCHAR(255) NOT NULL,
    content TEXT NOT NULL,
    published_at DATETIME DEFAULT NULL,
    category_id INT,
    user_id INT,
    FOREIGN KEY (category_id) REFERENCES Categories(category_id),
    FOREIGN KEY (user_id) REFERENCES Users(user_id)
)";
$conn->query($query);

// Table to store article-tag relationships
$query = "CREATE TABLE IF NOT EXISTS Article_Tags (
    article_id INT,
    tag_id INT,
    PRIMARY KEY (article_id, tag_id),
    FOREIGN KEY (article_id) REFERENCES Articles(article_id),
    FOREIGN KEY (tag_id) REFERENCES Tags(tag_id)
)";
$conn->query($query);

// Function to add a new article
function addArticle($title, $content, $category_id, $tags, $user_id) {
    global $conn;
    
    // Insert article
    $stmt = $conn->prepare("INSERT INTO Articles (title, content, category_id, user_id) VALUES (?, ?, ?, ?)");
    $stmt->bind_param("ssii", $title, $content, $category_id, $user_id);
    $stmt->execute();
    
    $article_id = $stmt->insert_id;  // Get last inserted article_id

    // Insert article-tag relationships
    foreach ($tags as $tag) {
        $tag_id = getTagId($tag);
        $stmt = $conn->prepare("INSERT INTO Article_Tags (article_id, tag_id) VALUES (?, ?)");
        $stmt->bind_param("ii", $article_id, $tag_id);
        $stmt->execute();
    }
    
    echo "Article created successfully!";
}

// Function to get tag ID, or insert new tag if it does not exist
function getTagId($tag_name) {
    global $conn;
    
    $stmt = $conn->prepare("SELECT tag_id FROM Tags WHERE tag_name = ?");
    $stmt->bind_param("s", $tag_name);
    $stmt->execute();
    $stmt->store_result();
    
    if ($stmt->num_rows == 0) {
        // Insert new tag if it doesn't exist
        $stmt = $conn->prepare("INSERT INTO Tags (tag_name) VALUES (?)");
        $stmt->bind_param("s", $tag_name);
        $stmt->execute();
        return $stmt->insert_id;
    }
    
    $stmt->bind_result($tag_id);
    $stmt->fetch();
    return $tag_id;
}

// Function to edit an existing article
function editArticle($article_id, $title, $content, $category_id, $tags) {
    global $conn;
    
    // Update article
    $stmt = $conn->prepare("UPDATE Articles SET title = ?, content = ?, category_id = ? WHERE article_id = ?");
    $stmt->bind_param("ssii", $title, $content, $category_id, $article_id);
    $stmt->execute();

    // Delete existing article tags and insert new ones
    $stmt = $conn->prepare("DELETE FROM Article_Tags WHERE article_id = ?");
    $stmt->bind_param("i", $article_id);
    $stmt->execute();
    
    foreach ($tags as $tag) {
        $tag_id = getTagId($tag);
        $stmt = $conn->prepare("INSERT INTO Article_Tags (article_id, tag_id) VALUES (?, ?)");
        $stmt->bind_param("ii", $article_id, $tag_id);
        $stmt->execute();
    }

    echo "Article updated successfully!";
}

// Function to publish an article
function publishArticle($article_id) {
    global $conn;
    
    // Set the published date
    $stmt = $conn->prepare("UPDATE Articles SET published_at = NOW() WHERE article_id = ?");
    $stmt->bind_param("i", $article_id);
    $stmt->execute();
    
    echo "Article published successfully!";
}

// Function to display all articles
function displayArticles() {
    global $conn;
    
    $query = "SELECT a.article_id, a.title, a.content, c.category_name, u.username, a.published_at 
              FROM Articles a
              JOIN Categories c ON a.category_id = c.category_id
              JOIN Users u ON a.user_id = u.user_id";
    $result = $conn->query($query);
    
    while ($row = $result->fetch_assoc()) {
        echo "Title: " . $row['title'] . "<br>";
        echo "Category: " . $row['category_name'] . "<br>";
        echo "Author: " . $row['username'] . "<br>";
        echo "Published at: " . $row['published_at'] . "<br><br>";
    }
}

// Example usage: Adding an article (you can later create HTML forms to handle user input)
addArticle("My First Article", "This is the content of my first article.", 1, ["Tech", "Innovation"], 1);

// Displaying articles
displayArticles();

?>