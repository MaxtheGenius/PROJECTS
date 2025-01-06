#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to estimate Pi using the Monte Carlo method
double estimatePi(int numPoints)
{
    int pointsInsideCircle = 0;

    // Loop through the number of points
    for (int i = 0; i < numPoints; i++)
    {
        // Generate random point (x, y) where -1 <= x, y <= 1
        double x = (2.0 * rand() / RAND_MAX) - 1.0;
        double y = (2.0 * rand() / RAND_MAX) - 1.0;

        // Check if the point is inside the unit circle
        if (x * x + y * y <= 1.0)
        {
            pointsInsideCircle++;
        }
    }

    // Estimate Pi using the ratio of points inside the circle
    return 4.0 * pointsInsideCircle / numPoints;
}

int main()
{
    // Seed the random number generator with the current time
    srand(time(NULL));

    // Number of random points to generate
    int numPoints = 1000000;

    // Estimate Pi
    double piEstimate = estimatePi(numPoints);

    // Print the estimated value of Pi
    printf("Estimated value of Pi: %.15f\n", piEstimate);

    return 0;
}