import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { FormsModule } from '@angular/forms';
import { AppComponent } from './app.component';
import { AppRoutingModule } from './app-routing.module';
import { VideoLessonsComponent } from './components/video-lessons/video-lessons.component';
import { InteractiveQuizzesComponent } from './components/interactive-quizzes/interactive-quizzes.component';
import { ProgressTrackingComponent } from './components/progress-tracking/progress-tracking.component';
import { DiscussionForumComponent } from './components/discussion-forum/discussion-forum.component';

@NgModule({
  declarations: [
    AppComponent,
    VideoLessonsComponent,
    InteractiveQuizzesComponent,
    ProgressTrackingComponent,
    DiscussionForumComponent,
  ],
  imports: [
    BrowserModule,
    FormsModule,
    AppRoutingModule,
  ],
  bootstrap: [AppComponent],
})
export class AppModule {}