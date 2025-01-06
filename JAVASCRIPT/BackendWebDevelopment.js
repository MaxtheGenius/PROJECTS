// Required modules
const express = require("express");
const bcrypt = require("bcrypt");
const jwt = require("jsonwebtoken");
const sqlite3 = require("sqlite3").verbose();
const bodyParser = require("body-parser");

// Configuration constants
const PORT = 3000;
const JWT_SECRET = "your_secret_key"; // Replace with a secure key in production
const SALT_ROUNDS = 10;

// Initialize Express app and middleware
const app = express();
app.use(bodyParser.json());

// Initialize SQLite database
const db = new sqlite3.Database(":memory:");

// Setup database schema
db.serialize(() => {
  db.run(`
    CREATE TABLE Users (
      user_id INTEGER PRIMARY KEY AUTOINCREMENT,
      username TEXT UNIQUE NOT NULL,
      password TEXT NOT NULL
    )
  `);

  db.run(`
    CREATE TABLE Rooms (
      room_id INTEGER PRIMARY KEY AUTOINCREMENT,
      room_number INTEGER NOT NULL,
      room_type TEXT NOT NULL,
      price_per_night REAL NOT NULL
    )
  `);

  db.run(`
    CREATE TABLE Bookings (
      booking_id INTEGER PRIMARY KEY AUTOINCREMENT,
      user_id INTEGER NOT NULL,
      room_id INTEGER NOT NULL,
      check_in_date TEXT NOT NULL,
      check_out_date TEXT NOT NULL,
      FOREIGN KEY (user_id) REFERENCES Users(user_id),
      FOREIGN KEY (room_id) REFERENCES Rooms(room_id)
    )
  `);

  // Insert sample data
  db.run(`
    INSERT INTO Rooms (room_number, room_type, price_per_night)
    VALUES 
    (101, 'Single', 100),
    (102, 'Double', 150),
    (103, 'Suite', 300)
  `);
});

// Utility function for generating JWT
const generateToken = (userId) => jwt.sign({ userId }, JWT_SECRET, { expiresIn: "1h" });

// Middleware for verifying JWT
const authenticate = (req, res, next) => {
  const token = req.headers.authorization?.split(" ")[1];
  if (!token) return res.status(401).json({ message: "Authorization token required" });

  jwt.verify(token, JWT_SECRET, (err, decoded) => {
    if (err) return res.status(403).json({ message: "Invalid or expired token" });
    req.userId = decoded.userId;
    next();
  });
};

// Route to register a new user
app.post("/register", (req, res) => {
  const { username, password } = req.body;
  if (!username || !password) return res.status(400).json({ message: "Username and password are required" });

  bcrypt.hash(password, SALT_ROUNDS, (err, hashedPassword) => {
    if (err) return res.status(500).json({ message: "Error encrypting password" });

    db.run(
      "INSERT INTO Users (username, password) VALUES (?, ?)",
      [username, hashedPassword],
      (err) => {
        if (err) return res.status(400).json({ message: "Username already exists" });
        res.status(201).json({ message: "User registered successfully" });
      }
    );
  });
});

// Route to login a user
app.post("/login", (req, res) => {
  const { username, password } = req.body;
  if (!username || !password) return res.status(400).json({ message: "Username and password are required" });

  db.get("SELECT * FROM Users WHERE username = ?", [username], (err, user) => {
    if (err || !user) return res.status(400).json({ message: "Invalid credentials" });

    bcrypt.compare(password, user.password, (err, match) => {
      if (err || !match) return res.status(400).json({ message: "Invalid credentials" });
      const token = generateToken(user.user_id);
      res.json({ token });
    });
  });
});

// Route to view available rooms
app.get("/rooms", authenticate, (req, res) => {
  db.all("SELECT * FROM Rooms", [], (err, rooms) => {
    if (err) return res.status(500).json({ message: "Error fetching rooms" });
    res.json(rooms);
  });
});

// Route to book a room
app.post("/bookings", authenticate, (req, res) => {
  const { room_id, check_in_date, check_out_date } = req.body;
  if (!room_id || !check_in_date || !check_out_date)
    return res.status(400).json({ message: "Room ID, check-in date, and check-out date are required" });

  db.run(
    `
    INSERT INTO Bookings (user_id, room_id, check_in_date, check_out_date)
    VALUES (?, ?, ?, ?)
  `,
    [req.userId, room_id, check_in_date, check_out_date],
    (err) => {
      if (err) return res.status(500).json({ message: "Error creating booking" });
      res.status(201).json({ message: "Booking created successfully" });
    }
  );
});

// Route to view user bookings
app.get("/bookings", authenticate, (req, res) => {
  db.all(
    `
    SELECT b.booking_id, r.room_number, r.room_type, r.price_per_night, b.check_in_date, b.check_out_date
    FROM Bookings b
    JOIN Rooms r ON b.room_id = r.room_id
    WHERE b.user_id = ?
  `,
    [req.userId],
    (err, bookings) => {
      if (err) return res.status(500).json({ message: "Error fetching bookings" });
      res.json(bookings);
    }
  );
});

// Start the server
app.listen(PORT, () => {
  console.log(`Server is running on http://localhost:${PORT}`);
});