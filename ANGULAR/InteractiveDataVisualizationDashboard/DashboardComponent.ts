// dashboard.component.ts
import { Component } from '@angular/core';

@Component({
  selector: 'app-dashboard',
  template: `
    <div class="dashboard">
      <div class="chart-container">
        <h2>Bar Chart: Sales Metrics</h2>
        <app-bar-chart [data]="salesData"></app-bar-chart>
      </div>
      <div class="chart-container">
        <h2>Line Chart: Revenue Trends</h2>
        <app-line-chart [data]="revenueData"></app-line-chart>
      </div>
    </div>
  `,
  styles: [`
    .dashboard {
      display: flex;
      flex-wrap: wrap;
      justify-content: space-around;
    }
    .chart-container {
      width: 45%;
      margin: 10px;
      border: 1px solid #ddd;
      padding: 10px;
      border-radius: 5px;
      background: #f9f9f9;
    }
    h2 {
      text-align: center;
      color: #333;
    }
  `],
})
export class DashboardComponent {
  salesData = this.generateRandomData(10, 1000, 'Category');
  revenueData = this.generateRandomData(12, 50000, 'Month');

  private generateRandomData(count: number, max: number, label: string) {
    return Array.from({ length: count }, (_, i) => ({
      name: `${label} ${i + 1}`,
      value: Math.floor(Math.random() * max),
    }));
  }
}