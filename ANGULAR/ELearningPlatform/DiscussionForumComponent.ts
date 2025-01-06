import { Component } from '@angular/core';

@Component({
  selector: 'app-discussion-forum',
  templateUrl: './discussion-forum.component.html',
  styleUrls: ['./discussion-forum.component.css'],
})
export class DiscussionForumComponent {
  posts = Array.from({ length: 3 }, (_, i) => ({
    author: `User ${i + 1}`,
    content: `This is a post from User ${i + 1}.`,
  }));
}