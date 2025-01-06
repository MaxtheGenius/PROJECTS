// Home page for the news blog

import React from 'react';
import Link from 'next/link';

/*
    Fetches the latest news articles from the News API.
    @returns {Object} The props for the page.
*/
export async function getServerSideProps() {
  // Fetch data from NewsAPI
  const res = await fetch('https://newsapi.org/v2/top-headlines?country=us&apiKey=YOUR_API_KEY');
  const data = await res.json();

  return {
    props: {
      articles: data.articles || [], // Return fetched articles
    },
  };
}

/*
    HomePage component displays a list of news articles.
    @param {Object} props The props containing articles data.
*/
const HomePage = ({ articles }) => {
  return (
    <div>
      {/* Header section */}
      <header style={{ textAlign: 'center', padding: '20px 0' }}>
        <h1>News Blog</h1>
        <p>Stay updated with the latest news around the world</p>
      </header>

      {/* Main content section with articles */}
      <main>
        <ul style={{ listStyle: 'none', padding: 0 }}>
          {articles.map((article, index) => (
            <li key={index} style={{ margin: '20px 0' }}>
              <h2>
                <Link href={`/article?title=${encodeURIComponent(article.title)}&content=${encodeURIComponent(article.content)}`}>
                  <a>{article.title}</a>
                </Link>
              </h2>
              <p>{article.description}</p>
            </li>
          ))}
        </ul>
      </main>

      {/* Footer section */}
      <footer style={{ textAlign: 'center', padding: '10px 0' }}>
        <p>&copy; {new Date().getFullYear()} News Blog</p>
      </footer>
    </div>
  );
};

export default HomePage;

// Individual article page for viewing full details

import React from 'react';
import { useRouter } from 'next/router';

/*
    ArticlePage component displays the full content of a selected article.
*/
const ArticlePage = () => {
  // Using useRouter to access query parameters
  const router = useRouter();
  const { title, content } = router.query;

  return (
    <div>
      {/* Header section for article title */}
      <header style={{ textAlign: 'center', padding: '20px 0' }}>
        <h1>{title}</h1>
      </header>

      {/* Main content section displaying article content */}
      <main style={{ maxWidth: '800px', margin: '0 auto', padding: '20px' }}>
        <p>{content}</p>
      </main>

      {/* Footer section */}
      <footer style={{ textAlign: 'center', padding: '10px 0' }}>
        <p>&copy; {new Date().getFullYear()} News Blog</p>
      </footer>
    </div>
  );
};

export default ArticlePage;

// Global styles for the blog

body {
    font-family: Arial, sans-serif;
    line-height: 1.6;
    margin: 0;
    padding: 0;
    background-color: #f9f9f9;
    color: #333;
  }
  
  /* Style for anchor links */
  a {
    text-decoration: none;
    color: #0070f3;
  }
  
  /* Hover effect for anchor links */
  a:hover {
    text-decoration: underline;
  }
  
  /* Header and footer background color */
  header,
  footer {
    background-color: #0070f3;
    color: white;
  }
  
  /* Main content padding */
  main {
    padding: 20px;
  }

// Configuration for the Next.js application

module.exports = {
    reactStrictMode: true,  // Enforce strict mode for React
  };