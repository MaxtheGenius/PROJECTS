<?php

// This application demonstrates caching and query optimization techniques.

// Step 1: Database Connection
class Database {
    private $pdo;

    public function __construct($host, $dbName, $user, $password) {
        $dsn = "mysql:host=$host;dbname=$dbName;charset=utf8mb4";
        $this->pdo = new PDO($dsn, $user, $password, [
            PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
            PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC
        ]);
    }

    // Optimized function to fetch data with prepared statements
    public function fetchData($query, $params = []) {
        $stmt = $this->pdo->prepare($query);
        $stmt->execute($params);
        return $stmt->fetchAll();
    }
}

// Step 2: File-Based Caching
class Cache {
    private $cacheDir = 'cache/';

    public function __construct() {
        if (!is_dir($this->cacheDir)) {
            mkdir($this->cacheDir, 0755, true);
        }
    }

    // Save data to cache
    public function set($key, $data, $ttl = 3600) {
        $cacheFile = $this->getCacheFile($key);
        $cacheData = [
            'data' => $data,
            'expires' => time() + $ttl
        ];
        file_put_contents($cacheFile, serialize($cacheData));
    }

    // Retrieve data from cache
    public function get($key) {
        $cacheFile = $this->getCacheFile($key);
        if (file_exists($cacheFile)) {
            $cacheData = unserialize(file_get_contents($cacheFile));
            if ($cacheData['expires'] > time()) {
                return $cacheData['data'];
            } else {
                unlink($cacheFile); // Remove expired cache
            }
        }
        return null;
    }

    private function getCacheFile($key) {
        return $this->cacheDir . md5($key) . '.cache';
    }
}

// Step 3: Application Logic
class ProductApp {
    private $db;
    private $cache;

    public function __construct(Database $db, Cache $cache) {
        $this->db = $db;
        $this->cache = $cache;
    }

    // Fetch product data with caching
    public function getProducts() {
        $cacheKey = 'products_list';
        $cachedData = $this->cache->get($cacheKey);

        if ($cachedData) {
            echo "Cache hit!\n";
            return $cachedData;
        } else {
            echo "Cache miss. Fetching from database...\n";
            $query = "SELECT id, name, price FROM products WHERE active = 1";
            $data = $this->db->fetchData($query);
            $this->cache->set($cacheKey, $data, 600); // Cache for 10 minutes
            return $data;
        }
    }

    // Function to display product data
    public function displayProducts() {
        $products = $this->getProducts();
        foreach ($products as $product) {
            echo "ID: {$product['id']}, Name: {$product['name']}, Price: {$product['price']}\n";
        }
    }
}

// Step 4: Main Script Execution
try {
    // Database credentials
    $dbHost = 'localhost';
    $dbName = 'ecommerce';
    $dbUser = 'root';
    $dbPassword = '';

    // Initialize database and cache
    $database = new Database($dbHost, $dbName, $dbUser, $dbPassword);
    $cache = new Cache();

    // Create application instance
    $app = new ProductApp($database, $cache);

    // Display products
    $app->displayProducts();

} catch (Exception $e) {
    echo "Error: " . $e->getMessage();
}
?>