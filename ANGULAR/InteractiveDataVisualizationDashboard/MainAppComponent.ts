// app.component.ts
import { Component } from '@angular/core';

@Component({
  selector: 'app-root',
  template: `
    <div class="app-container">
      <h1>Dynamic Data Visualization Dashboard</h1>
      <app-dashboard></app-dashboard>
    </div>
  `,
  styles: [`
    .app-container {
      text-align: center;
      font-family: Arial, sans-serif;
    }
    h1 {
      margin: 10px;
      color: #007acc;
    }
  `],
})
export class AppComponent {}