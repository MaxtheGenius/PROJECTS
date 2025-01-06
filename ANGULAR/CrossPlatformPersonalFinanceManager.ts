// app.module.ts
import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { IonicModule } from '@ionic/angular';
import { FormsModule } from '@angular/forms';
import { AppComponent } from './app.component';
import { FinanceDashboardComponent } from './finance-dashboard/finance-dashboard.component';

@NgModule({
  declarations: [
    AppComponent,
    FinanceDashboardComponent,
  ],
  imports: [
    BrowserModule,
    IonicModule.forRoot(),
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
    <ion-app>
      <ion-header>
        <ion-toolbar>
          <ion-title>Personal Finance Manager</ion-title>
        </ion-toolbar>
      </ion-header>
      <ion-content>
        <app-finance-dashboard></app-finance-dashboard>
      </ion-content>
    </ion-app>
  `,
  styles: [`
    ion-header {
      background-color: #3880ff;
      color: white;
    }
    ion-title {
      text-align: center;
    }
  `],
})
export class AppComponent { }

// finance-dashboard.component.ts
import { Component, OnInit } from '@angular/core';

interface Expense {
  id: string;
  category: string;
  amount: number;
  date: string;
}

interface Budget {
  category: string;
  allocated: number;
  spent: number;
}

interface Investment {
  id: string;
  type: string;
  value: number;
  growth: string;
}

@Component({
  selector: 'app-finance-dashboard',
  template: `
    <div class="dashboard">
      <ion-segment [(ngModel)]="activeTab">
        <ion-segment-button value="expenses">Expenses</ion-segment-button>
        <ion-segment-button value="budget">Budget</ion-segment-button>
        <ion-segment-button value="investments">Investments</ion-segment-button>
      </ion-segment>

      <div *ngIf="activeTab === 'expenses'" class="tab-content">
        <h2>Expense Tracking</h2>
        <ion-list>
          <ion-item *ngFor="let expense of expenses">
            <ion-label>
              {{ expense.category }}: ${{ expense.amount.toFixed(2) }} on {{ expense.date }}
            </ion-label>
          </ion-item>
        </ion-list>
        <ion-button expand="full" (click)="addRandomExpense()">Add Random Expense</ion-button>
      </div>

      <div *ngIf="activeTab === 'budget'" class="tab-content">
        <h2>Budget Planning</h2>
        <ion-list>
          <ion-item *ngFor="let budget of budgets">
            <ion-label>
              {{ budget.category }}: Allocated ${{ budget.allocated.toFixed(2) }}, Spent ${{ budget.spent.toFixed(2) }}
            </ion-label>
          </ion-item>
        </ion-list>
        <ion-button expand="full" (click)="addRandomBudget()">Add Random Budget</ion-button>
      </div>

      <div *ngIf="activeTab === 'investments'" class="tab-content">
        <h2>Investment Portfolio</h2>
        <ion-list>
          <ion-item *ngFor="let investment of investments">
            <ion-label>
              {{ investment.type }}: Value ${{ investment.value.toFixed(2) }}, Growth {{ investment.growth }}
            </ion-label>
          </ion-item>
        </ion-list>
        <ion-button expand="full" (click)="addRandomInvestment()">Add Random Investment</ion-button>
      </div>
    </div>
  `,
  styles: [`
    .dashboard {
      margin: 20px;
    }
    .tab-content {
      margin-top: 20px;
    }
    h2 {
      color: #3880ff;
    }
  `],
})
export class FinanceDashboardComponent implements OnInit {
  activeTab = 'expenses';
  expenses: Expense[] = [];
  budgets: Budget[] = [];
  investments: Investment[] = [];

  ngOnInit() {
    // Initialize with random data
    this.expenses = this.generateRandomExpenses(5);
    this.budgets = this.generateRandomBudgets(3);
    this.investments = this.generateRandomInvestments(4);
  }

  generateRandomExpenses(count: number): Expense[] {
    const categories = ['Food', 'Transport', 'Entertainment', 'Utilities'];
    return Array.from({ length: count }, (_, i) => ({
      id: `exp${i}`,
      category: categories[Math.floor(Math.random() * categories.length)],
      amount: Math.random() * 100,
      date: new Date().toISOString().split('T')[0],
    }));
  }

  generateRandomBudgets(count: number): Budget[] {
    const categories = ['Food', 'Transport', 'Entertainment', 'Utilities'];
    return Array.from({ length: count }, (_, i) => ({
      category: categories[Math.floor(Math.random() * categories.length)],
      allocated: Math.random() * 500 + 100,
      spent: Math.random() * 500,
    }));
  }

  generateRandomInvestments(count: number): Investment[] {
    const types = ['Stocks', 'Bonds', 'Real Estate', 'Crypto'];
    const growthOptions = ['+2%', '+5%', '-1%', '+10%'];
    return Array.from({ length: count }, (_, i) => ({
      id: `inv${i}`,
      type: types[Math.floor(Math.random() * types.length)],
      value: Math.random() * 10000,
      growth: growthOptions[Math.floor(Math.random() * growthOptions.length)],
    }));
  }

  addRandomExpense() {
    this.expenses.push(this.generateRandomExpenses(1)[0]);
  }

  addRandomBudget() {
    this.budgets.push(this.generateRandomBudgets(1)[0]);
  }

  addRandomInvestment() {
    this.investments.push(this.generateRandomInvestments(1)[0]);
  }
}