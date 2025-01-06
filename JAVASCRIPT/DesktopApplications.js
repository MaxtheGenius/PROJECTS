// Electron Task Manager Application

// This script combines all functionalities: window management, frontend, backend integration, and cloud sync.

// Import necessary modules
const { app, BrowserWindow, ipcMain, Notification } = require("electron");
const path = require("path");
const axios = require("axios");

// Mock cloud API URL for task synchronization
const CLOUD_API_URL = "https://example.com/api/tasks"; // Replace with actual backend API URL

// Variable to hold the main window instance
let mainWindow;

// Function to create the main application window
function createMainWindow() {
    mainWindow = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            preload: path.join(__dirname, "preload.js"), // Preload script for secure context
            nodeIntegration: true, // Enable Node.js integration
            contextIsolation: false // Allow communication between renderer and main processes
        }
    });

    mainWindow.loadFile("index.html"); // Load the frontend UI
}

// Electron app lifecycle events
app.whenReady().then(() => {
    createMainWindow(); // Create the main window when the app is ready

    // Re-create the window on macOS when the app is re-activated
    app.on("activate", () => {
        if (BrowserWindow.getAllWindows().length === 0) createMainWindow();
    });
});

// Quit the app when all windows are closed (except on macOS)
app.on("window-all-closed", () => {
    if (process.platform !== "darwin") app.quit();
});

// Handle task notifications
ipcMain.on("task-notification", (event, task) => {
    new Notification({
        title: "Task Reminder",
        body: `Reminder: "${task.name}" is due at ${task.dueDate}`
    }).show();
});

// Handle task synchronization with the cloud
ipcMain.on("sync-tasks", async (event, tasks) => {
    try {
        await axios.post(`${CLOUD_API_URL}/sync`, { tasks });
        event.reply("sync-status", { success: true, message: "Tasks synced successfully!" });
    } catch (error) {
        event.reply("sync-status", { success: false, message: "Failed to sync tasks. Please try again." });
    }
});

// Preload script to enable secure context in the renderer process
const preloadScript = `
const { ipcRenderer } = require("electron");
window.ipcRenderer = ipcRenderer;
`;

// Renderer HTML and frontend logic (combined for simplicity)
const rendererHTML = `
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Task Manager</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 0; padding: 0; display: flex; flex-direction: column; align-items: center; background-color: #f0f0f0; height: 100vh; }
        h1 { margin-top: 20px; }
        form, #tasks, button { width: 80%; margin-bottom: 20px; }
        .task { display: flex; justify-content: space-between; padding: 10px; background: white; border: 1px solid #ddd; border-radius: 5px; margin-bottom: 10px; }
    </style>
</head>
<body>
    <h1>Task Manager</h1>
    <form id="task-form">
        <input type="text" id="task-name" placeholder="Task Name" required />
        <input type="datetime-local" id="task-due" required />
        <button type="submit">Add Task</button>
    </form>
    <div id="tasks"></div>
    <button id="sync-tasks">Sync Tasks</button>

    <script>
        const ipc = window.ipcRenderer;
        const taskForm = document.getElementById("task-form");
        const taskNameInput = document.getElementById("task-name");
        const taskDueInput = document.getElementById("task-due");
        const tasksDiv = document.getElementById("tasks");
        const syncButton = document.getElementById("sync-tasks");

        let tasks = [];

        // Add a new task
        taskForm.addEventListener("submit", (e) => {
            e.preventDefault();
            const task = { name: taskNameInput.value, dueDate: taskDueInput.value };
            tasks.push(task);
            displayTasks();
            ipc.send("task-notification", task); // Send task notification
            taskForm.reset();
        });

        // Display tasks in the UI
        function displayTasks() {
            tasksDiv.innerHTML = "";
            tasks.forEach((task, index) => {
                const taskDiv = document.createElement("div");
                taskDiv.className = "task";
                taskDiv.innerHTML = \`
                    <span>\${task.name} - Due: \${task.dueDate}</span>
                    <button onclick="deleteTask(\${index})">Delete</button>
                \`;
                tasksDiv.appendChild(taskDiv);
            });
        }

        // Delete a task
        window.deleteTask = (index) => {
            tasks.splice(index, 1);
            displayTasks();
        };

        // Sync tasks with the backend
        syncButton.addEventListener("click", () => {
            ipc.send("sync-tasks", tasks);
        });

        // Handle sync status
        ipc.on("sync-status", (event, status) => {
            alert(status.message);
        });
    </script>
</body>
</html>
`;

// Serve the HTML as part of the Electron window
const fs = require("fs");
const htmlPath = path.join(__dirname, "index.html");
const preloadPath = path.join(__dirname, "preload.js");

// Write HTML and Preload script to disk
fs.writeFileSync(htmlPath, rendererHTML, "utf-8");
fs.writeFileSync(preloadPath, preloadScript, "utf-8");