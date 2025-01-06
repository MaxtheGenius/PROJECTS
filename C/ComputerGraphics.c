#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

// Function to render a simple triangle
void renderTriangle()
{
    glBegin(GL_TRIANGLES);    // Start drawing a triangle
    glColor3f(1.0, 0.0, 0.0); // Set color to red
    glVertex2f(0.0f, 0.5f);   // Top vertex
    glVertex2f(-0.5f, -0.5f); // Bottom-left vertex
    glVertex2f(0.5f, -0.5f);  // Bottom-right vertex
    glEnd();
}

// Function to render a simple square
void renderSquare()
{
    glBegin(GL_QUADS);        // Start drawing a square
    glColor3f(0.0, 1.0, 0.0); // Set color to green
    glVertex2f(-0.5f, 0.5f);  // Top-left vertex
    glVertex2f(0.5f, 0.5f);   // Top-right vertex
    glVertex2f(0.5f, -0.5f);  // Bottom-right vertex
    glVertex2f(-0.5f, -0.5f); // Bottom-left vertex
    glEnd();
}

// Function to set up the OpenGL environment
void setupOpenGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black
    glMatrixMode(GL_PROJECTION);          // Set projection matrix
    glLoadIdentity();                     // Reset projection matrix
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);     // Set 2D orthogonal projection
}

// The display callback function
void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
    renderTriangle();             // Render the triangle
    renderSquare();               // Render the square
    glFlush();                    // Ensure the rendering is done
    glutSwapBuffers();            // Swap the buffers (for double buffering)
}

// The main function
int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Double buffering and RGB color mode
    glutInitWindowSize(800, 600);                // Set the window size
    glutCreateWindow("OpenGL Simple Shapes");    // Create the window with the title

    setupOpenGL(); // Set up the OpenGL environment

    glutDisplayFunc(display); // Register the display callback function

    // Enter the GLUT main loop
    glutMainLoop();

    return 0;
}