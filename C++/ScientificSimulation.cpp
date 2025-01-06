#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <atomic>
#include <mutex>

using namespace std;

// Constants
const double ELECTRIC_CHARGE = 1.602e-19; // Coulombs (charge of an electron/proton)
const double PI = 3.14159265358979323846;

// Define a structure to represent a 3D vector
struct Vector3D {
    double x, y, z;

    Vector3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    // Vector addition
    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }

    // Scalar multiplication
    Vector3D operator*(double scalar) const {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    // Dot product
    double dot(const Vector3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Cross product
    Vector3D cross(const Vector3D& other) const {
        return Vector3D(y * other.z - z * other.y,
                        z * other.x - x * other.z,
                        x * other.y - y * other.x);
    }

    // Magnitude
    double magnitude() const {
        return sqrt(x * x + y * y + z * z);
    }

    // Normalize vector
    Vector3D normalize() const {
        double mag = magnitude();
        return *this * (1.0 / mag);
    }
};

// Particle class to represent each particle in the accelerator
class Particle {
public:
    Vector3D position;       // Position in 3D space
    Vector3D velocity;       // Velocity in 3D space
    Vector3D force;          // Force acting on the particle
    double mass;             // Mass of the particle
    double charge;           // Charge of the particle

    Particle(Vector3D position, Vector3D velocity, double mass, double charge)
        : position(position), velocity(velocity), mass(mass), charge(charge) {}

    // Update the particle's position and velocity based on the applied force
    void update(double dt) {
        // F = ma, so acceleration = force / mass
        Vector3D acceleration = force * (1.0 / mass);
        velocity = velocity + acceleration * dt;    // v = v0 + a*t
        position = position + velocity * dt;        // x = x0 + v*t

        // Reset force for the next calculation
        force = Vector3D(0, 0, 0);
    }

    // Apply force to the particle
    void applyForce(const Vector3D& f) {
        force = force + f;
    }
};

// Class to simulate the particle accelerator system
class ParticleAccelerator {
public:
    vector<Particle> particles;           // List of particles in the accelerator
    double time_step;                     // Time step for simulation
    atomic<bool> simulation_running;      // Flag to control simulation state

    ParticleAccelerator(double time_step = 1e-9) : time_step(time_step), simulation_running(false) {}

    // Add a particle to the system
    void addParticle(const Particle& p) {
        particles.push_back(p);
    }

    // Simulate the movement of particles and handle interactions
    void simulate() {
        simulation_running = true;
        vector<thread> simulation_threads;

        // Parallelize the update process for performance optimization
        for (size_t i = 0; i < particles.size(); ++i) {
            simulation_threads.push_back(thread(&ParticleAccelerator::updateParticle, this, i));
        }

        // Join all threads
        for (auto& th : simulation_threads) {
            th.join();
        }

        // Once all particles are updated, apply interactions (simple placeholder for more complex physics)
        for (size_t i = 0; i < particles.size(); ++i) {
            handleInteractions(i);
        }

        simulation_running = false;
    }

    // Update a particle (run in parallel)
    void updateParticle(size_t index) {
        particles[index].update(time_step);
    }

    // Handle simple interactions (e.g., electromagnetic force) between particles
    void handleInteractions(size_t index) {
        Particle& particle = particles[index];

        // Loop through other particles and apply a simple interaction (Coulomb's law)
        for (size_t i = 0; i < particles.size(); ++i) {
            if (i != index) {
                Particle& other = particles[i];

                // Calculate distance vector between particles
                Vector3D distance = other.position - particle.position;
                double distance_magnitude = distance.magnitude();

                if (distance_magnitude > 0.1) {  // Avoid division by zero for very close particles
                    // Coulomb's Law: F = k * (q1 * q2) / r^2
                    double force_magnitude = (ELECTRIC_CHARGE * ELECTRIC_CHARGE) / (distance_magnitude * distance_magnitude);
                    Vector3D force_direction = distance.normalize();
                    Vector3D force = force_direction * force_magnitude;

                    // Apply force to the particle
                    particle.applyForce(force);
                }
            }
        }
    }

    // Display the particle system's state
    void displayState() {
        for (size_t i = 0; i < particles.size(); ++i) {
            const Particle& p = particles[i];
            cout << "Particle " << i << " | Position: (" 
                 << p.position.x << ", " << p.position.y << ", " << p.position.z 
                 << ") | Velocity: (" << p.velocity.x << ", " << p.velocity.y << ", " << p.velocity.z 
                 << ")" << endl;
        }
    }
};

// Main function to demonstrate the particle accelerator simulation
int main() {
    // Initialize the particle accelerator with a time step
    ParticleAccelerator accelerator(1e-9);

    // Add particles to the system
    accelerator.addParticle(Particle(Vector3D(0, 0, 0), Vector3D(0, 0, 0), 1e-10, ELECTRIC_CHARGE));
    accelerator.addParticle(Particle(Vector3D(1, 0, 0), Vector3D(0, 0, 0), 1e-10, -ELECTRIC_CHARGE));
    accelerator.addParticle(Particle(Vector3D(0, 1, 0), Vector3D(0, 0, 0), 1e-10, ELECTRIC_CHARGE));

    // Simulate particle movement and interactions for a number of steps
    for (int step = 0; step < 1000; ++step) {
        accelerator.simulate();

        // Display the current state of the particles
        cout << "Step " << step << " state:" << endl;
        accelerator.displayState();
    }

    return 0;
}