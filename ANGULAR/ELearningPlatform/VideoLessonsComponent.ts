import { Component } from '@angular/core';

@Component({
  selector: 'app-video-lessons',
  templateUrl: './video-lessons.component.html',
  styleUrls: ['./video-lessons.component.css'],
})
export class VideoLessonsComponent {
  lessons = Array.from({ length: 5 }, (_, i) => ({
    title: `Lesson ${i + 1}`,
    description: `Description of lesson ${i + 1}`,
    videoUrl: `https://via.placeholder.com/400?text=Video+${i + 1}`,
  }));
}