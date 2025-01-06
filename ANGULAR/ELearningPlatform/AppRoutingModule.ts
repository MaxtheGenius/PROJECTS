import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { VideoLessonsComponent } from './components/video-lessons/video-lessons.component';
import { InteractiveQuizzesComponent } from './components/interactive-quizzes/interactive-quizzes.component';
import { ProgressTrackingComponent } from './components/progress-tracking/progress-tracking.component';
import { DiscussionForumComponent } from './components/discussion-forum/discussion-forum.component';

const routes: Routes = [
  { path: '', redirectTo: '/video-lessons', pathMatch: 'full' },
  { path: 'video-lessons', component: VideoLessonsComponent },
  { path: 'interactive-quizzes', component: InteractiveQuizzesComponent },
  { path: 'progress-tracking', component: ProgressTrackingComponent },
  { path: 'discussion-forum', component: DiscussionForumComponent },
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule],
})
export class AppRoutingModule {}