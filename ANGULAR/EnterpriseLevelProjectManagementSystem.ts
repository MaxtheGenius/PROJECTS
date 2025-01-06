// app.module.ts
import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { FormsModule } from '@angular/forms';
import { AppComponent } from './app.component';
import { ProjectDashboardComponent } from './project-dashboard/project-dashboard.component';

@NgModule({
  declarations: [
    AppComponent,
    ProjectDashboardComponent,
  ],
  imports: [
    BrowserModule,
    FormsModule,
  ],
  providers: [],
  bootstrap: [AppComponent],
})
export class AppModule { }

// app.component.ts
import { Component } from '@angular/core';

@Component({
  selector: 'app-root',
  template: `
    <div class="app-container">
      <h1>Enterprise Project Management System</h1>
      <app-project-dashboard></app-project-dashboard>
    </div>
  `,
  styles: [`
    .app-container {
      font-family: Arial, sans-serif;
      text-align: center;
    }
    h1 {
      background-color: #007bff;
      color: white;
      padding: 10px;
      margin: 0;
    }
  `],
})
export class AppComponent { }

// project-dashboard.component.ts
import { Component, OnInit } from '@angular/core';

interface TeamMember {
  id: string;
  name: string;
  role: string;
}

interface Task {
  id: string;
  title: string;
  assignedTo: string;
  progress: number;
  deadline: string;
}

interface Project {
  id: string;
  name: string;
  description: string;
  tasks: Task[];
  team: TeamMember[];
}

@Component({
  selector: 'app-project-dashboard',
  template: `
    <div class="dashboard">
      <h2>Projects</h2>
      <div *ngFor="let project of projects" class="project-card">
        <h3>{{ project.name }}</h3>
        <p>{{ project.description }}</p>
        <h4>Tasks</h4>
        <table>
          <thead>
            <tr>
              <th>Title</th>
              <th>Assigned To</th>
              <th>Progress</th>
              <th>Deadline</th>
            </tr>
          </thead>
          <tbody>
            <tr *ngFor="let task of project.tasks">
              <td>{{ task.title }}</td>
              <td>{{ task.assignedTo }}</td>
              <td>
                <div class="progress-bar">
                  <div class="progress" [style.width.%]="task.progress">{{ task.progress }}%</div>
                </div>
              </td>
              <td>{{ task.deadline }}</td>
            </tr>
          </tbody>
        </table>
        <h4>Team</h4>
        <ul>
          <li *ngFor="let member of project.team">{{ member.name }} - {{ member.role }}</li>
        </ul>
      </div>
      <button (click)="addRandomProject()">Add Random Project</button>
    </div>
  `,
  styles: [`
    .dashboard {
      margin: 20px;
    }
    .project-card {
      border: 1px solid #ddd;
      margin: 10px;
      padding: 10px;
    }
    h3 {
      margin: 0;
      color: #007bff;
    }
    table {
      width: 100%;
      border-collapse: collapse;
      margin-bottom: 10px;
    }
    th, td {
      border: 1px solid #ddd;
      padding: 5px;
    }
    .progress-bar {
      background: #e9ecef;
      border-radius: 4px;
      overflow: hidden;
    }
    .progress {
      background: #007bff;
      color: white;
      text-align: center;
      padding: 2px;
    }
    ul {
      list-style: none;
      padding: 0;
    }
    li {
      margin: 5px 0;
    }
    button {
      background: #007bff;
      color: white;
      border: none;
      padding: 10px 20px;
      cursor: pointer;
      border-radius: 4px;
    }
    button:hover {
      background: #0056b3;
    }
  `],
})
export class ProjectDashboardComponent implements OnInit {
  projects: Project[] = [];

  ngOnInit() {
    // Initialize with random projects
    this.projects = this.generateRandomProjects(3);
  }

  generateRandomProjects(count: number): Project[] {
    const projectNames = ['Website Redesign', 'Marketing Campaign', 'App Development'];
    const roles = ['Developer', 'Designer', 'Manager', 'Tester'];
    const names = ['Alice', 'Bob', 'Charlie', 'Dana'];

    return Array.from({ length: count }, (_, i) => ({
      id: `proj${i}`,
      name: projectNames[Math.floor(Math.random() * projectNames.length)],
      description: `Description for project ${i + 1}`,
      tasks: Array.from({ length: 5 }, (_, j) => ({
        id: `task${j}`,
        title: `Task ${j + 1}`,
        assignedTo: names[Math.floor(Math.random() * names.length)],
        progress: Math.floor(Math.random() * 100),
        deadline: new Date(new Date().getTime() + Math.random() * 1e10).toISOString().split('T')[0],
      })),
      team: Array.from({ length: 4 }, (_, k) => ({
        id: `member${k}`,
        name: names[k],
        role: roles[Math.floor(Math.random() * roles.length)],
      })),
    }));
  }

  addRandomProject() {
    this.projects.push(this.generateRandomProjects(1)[0]);
  }
}