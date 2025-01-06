import { Component } from '@angular/core';

@Component({
  selector: 'app-progress-tracking',
  templateUrl: './progress-tracking.component.html',
  styleUrls: ['./progress-tracking.component.css'],
})
export class ProgressTrackingComponent {
  progress = Array.from({ length: 5 }, (_, i) => ({
    lesson: `Lesson ${i + 1}`,
    completed: Math.random() > 0.5,
  }));
}