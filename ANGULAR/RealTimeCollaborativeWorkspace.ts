// app.module.ts
import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { FormsModule } from '@angular/forms';
import { HttpClientModule } from '@angular/common/http';
import { AppComponent } from './app.component';
import { WorkspaceComponent } from './workspace/workspace.component';

@NgModule({
  declarations: [
    AppComponent,
    WorkspaceComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    HttpClientModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }

// app.component.ts
import { Component } from '@angular/core';

@Component({
  selector: 'app-root',
  template: `
    <div class="app-container">
      <h1>Collaborative Workspace</h1>
      <app-workspace></app-workspace>
    </div>
  `,
  styles: [`
    .app-container {
      text-align: center;
      margin: 20px;
      font-family: Arial, sans-serif;
    }
    h1 {
      color: #4CAF50;
    }
  `]
})
export class AppComponent { }

// workspace.component.ts
import { Component, OnInit } from '@angular/core';

interface Project {
  id: string;
  name: string;
  type: 'Document' | 'Diagram' | 'Code';
  content: string;
}

@Component({
  selector: 'app-workspace',
  template: `
    <div class="workspace">
      <div class="project-list">
        <h2>Projects</h2>
        <ul>
          <li *ngFor="let project of projects" (click)="selectProject(project)" [class.selected]="project.id === selectedProject?.id">
            {{ project.name }} ({{ project.type }})
          </li>
        </ul>
        <button (click)="addRandomProject()">Add Random Project</button>
      </div>

      <div class="editor" *ngIf="selectedProject">
        <h2>{{ selectedProject.name }} - {{ selectedProject.type }}</h2>
        <textarea [(ngModel)]="selectedProject.content" (input)="updateContent()" rows="15"></textarea>
        <p><strong>Real-time Updates:</strong> {{ realTimeMessage }}</p>
      </div>
    </div>
  `,
  styles: [`
    .workspace {
      display: flex;
      justify-content: space-between;
      margin: 20px;
    }
    .project-list {
      width: 30%;
      background-color: #f9f9f9;
      padding: 10px;
      border-radius: 5px;
      box-shadow: 0 0 5px rgba(0, 0, 0, 0.1);
    }
    .editor {
      width: 65%;
    }
    ul {
      list-style: none;
      padding: 0;
    }
    li {
      padding: 5px;
      cursor: pointer;
    }
    li.selected {
      background-color: #4CAF50;
      color: white;
      border-radius: 5px;
    }
    textarea {
      width: 100%;
      padding: 10px;
      font-family: monospace;
    }
    button {
      margin-top: 10px;
      padding: 10px;
      background-color: #4CAF50;
      color: white;
      border: none;
      border-radius: 5px;
      cursor: pointer;
    }
    button:hover {
      background-color: #45a049;
    }
  `]
})
export class WorkspaceComponent implements OnInit {
  projects: Project[] = [];
  selectedProject: Project | null = null;
  realTimeMessage = 'Waiting for updates...';

  ngOnInit() {
    // Load initial random projects
    this.projects = this.generateRandomProjects(5);
  }

  generateRandomProjects(count: number): Project[] {
    const types: Array<'Document' | 'Diagram' | 'Code'> = ['Document', 'Diagram', 'Code'];
    const randomProjects: Project[] = [];
    for (let i = 0; i < count; i++) {
      randomProjects.push({
        id: `${Math.random().toString(36).substr(2, 9)}`,
        name: `Project ${i + 1}`,
        type: types[Math.floor(Math.random() * types.length)],
        content: `Sample content for Project ${i + 1}`
      });
    }
    return randomProjects;
  }

  selectProject(project: Project) {
    this.selectedProject = project;
    this.realTimeMessage = 'You are now editing this project.';
  }

  updateContent() {
    // Simulate real-time updates
    this.realTimeMessage = `Last update: ${new Date().toLocaleTimeString()}`;
  }

  addRandomProject() {
    const newProject = this.generateRandomProjects(1)[0];
    this.projects.push(newProject);
  }
}