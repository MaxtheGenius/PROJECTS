async function fetchUsers() {
    const users = await window.__TAURI__.invoke("generate_users");
    const userList = document.getElementById("user-list");
    userList.innerHTML = "";
    users.forEach((user) => {
      const li = document.createElement("li");
      li.textContent = `${user.name} (${user.email})`;
      userList.appendChild(li);
    });
  }
  
  async function fetchTasks() {
    const tasks = await window.__TAURI__.invoke("generate_tasks");
    const taskList = document.getElementById("task-list");
    taskList.innerHTML = "";
    tasks.forEach((task) => {
      const li = document.createElement("li");
      li.textContent = `${task.title}: ${task.description} (Completed: ${task.completed})`;
      taskList.appendChild(li);
    });
  }
  
  document.addEventListener("DOMContentLoaded", () => {
    fetchUsers();
    fetchTasks();
  });  