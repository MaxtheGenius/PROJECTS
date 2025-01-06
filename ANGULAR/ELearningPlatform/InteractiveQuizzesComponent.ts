import { Component } from '@angular/core';

@Component({
  selector: 'app-interactive-quizzes',
  templateUrl: './interactive-quizzes.component.html',
  styleUrls: ['./interactive-quizzes.component.css'],
})
export class InteractiveQuizzesComponent {
  questions = Array.from({ length: 3 }, (_, i) => ({
    question: `Question ${i + 1}`,
    options: ['Option A', 'Option B', 'Option C', 'Option D'],
    correct: 'Option B',
  }));
}