#include <stdio.h>
#include <stdlib.h>

// Function to simulate the vehicle's suspension system
void simulateSuspension(double mass, double springConstant, double dampingCoefficient, double initialDisplacement, double initialVelocity, double timeStep, int numSteps)
{
    double displacement = initialDisplacement;
    double velocity = initialVelocity;
    double acceleration;
    double forceSpring, forceDamping;

    // Simulation loop
    for (int step = 0; step < numSteps; step++)
    {
        // Calculate the spring force: F = -k * x
        forceSpring = -springConstant * displacement;

        // Calculate the damping force: F = -b * v
        forceDamping = -dampingCoefficient * velocity;

        // Total force on the suspension system: F = m * a
        acceleration = (forceSpring + forceDamping) / mass;

        // Update the velocity and displacement using the Euler method
        velocity += acceleration * timeStep;
        displacement += velocity * timeStep;

        // Output the state of the suspension at each time step
        printf("Time: %.2f s, Displacement: %.4f m, Velocity: %.4f m/s, Acceleration: %.4f m/s^2\n", step * timeStep, displacement, velocity, acceleration);
    }
}

int main()
{
    // Parameters for the suspension system
    double mass = 1500.0;               // Mass of the vehicle (kg)
    double springConstant = 25000.0;    // Spring constant (N/m)
    double dampingCoefficient = 1500.0; // Damping coefficient (Ns/m)
    double initialDisplacement = 0.1;   // Initial displacement (m) (e.g., 10 cm compression)
    double initialVelocity = 0.0;       // Initial velocity (m/s)
    double timeStep = 0.01;             // Time step for simulation (seconds)
    int numSteps = 1000;                // Number of simulation steps

    // Run the simulation
    simulateSuspension(mass, springConstant, dampingCoefficient, initialDisplacement, initialVelocity, timeStep, numSteps);

    return 0;
}