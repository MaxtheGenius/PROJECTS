#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Structure representing a 2D vector
struct Vector2D {
    float x; // X-coordinate
    float y; // Y-coordinate

    // Constructor
    Vector2D(float x = 0, float y = 0) : x(x), y(y) {}

    // Add two vectors
    Vector2D operator+(const Vector2D &other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    // Subtract two vectors
    Vector2D operator-(const Vector2D &other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    // Multiply vector by a scalar
    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    // Calculate the magnitude of the vector
    float magnitude() const {
        return sqrt(x * x + y * y);
    }

    // Normalize the vector
    Vector2D normalize() const {
        float mag = magnitude();
        return (mag > 0) ? Vector2D(x / mag, y / mag) : Vector2D(0, 0);
    }

    // Dot product of two vectors
    float dot(const Vector2D &other) const {
        return x * other.x + y * other.y;
    }
};

// Structure representing a 2D circle
struct Circle {
    Vector2D position; // Position of the circle
    Vector2D velocity; // Velocity of the circle
    float radius;      // Radius of the circle
    float mass;        // Mass of the circle

    // Constructor
    Circle(Vector2D pos, Vector2D vel, float rad, float m)
        : position(pos), velocity(vel), radius(rad), mass(m) {}
};

// Check for collision between two circles
bool checkCollision(const Circle &a, const Circle &b) {
    float distance = (a.position - b.position).magnitude();
    return distance < (a.radius + b.radius); // Collision if distance is less than combined radii
}

// Resolve collision between two circles
void resolveCollision(Circle &a, Circle &b) {
    Vector2D normal = (b.position - a.position).normalize();
    Vector2D relativeVelocity = b.velocity - a.velocity;
    float velocityAlongNormal = relativeVelocity.dot(normal);

    // Ignore collisions if objects are separating
    if (velocityAlongNormal > 0) {
        return;
    }

    // Calculate restitution (bounciness), set to 1 for perfect elastic collision
    float restitution = 1.0f;

    // Calculate impulse scalar
    float impulse = -(1 + restitution) * velocityAlongNormal;
    impulse /= (1 / a.mass) + (1 / b.mass);

    // Apply impulse to objects
    Vector2D impulseVector = normal * impulse;
    a.velocity = a.velocity - impulseVector * (1 / a.mass);
    b.velocity = b.velocity + impulseVector * (1 / b.mass);
}

// Simulate one step of the physics engine
void simulateStep(vector<Circle> &circles, float deltaTime) {
    // Update positions
    for (auto &circle : circles) {
        circle.position = circle.position + circle.velocity * deltaTime;
    }

    // Check for collisions and resolve them
    for (size_t i = 0; i < circles.size(); ++i) {
        for (size_t j = i + 1; j < circles.size(); ++j) {
            if (checkCollision(circles[i], circles[j])) {
                resolveCollision(circles[i], circles[j]);
            }
        }
    }
}

// Main function
int main() {
    // Define simulation parameters
    float deltaTime = 0.01f; // Time step for simulation

    // Create circles
    vector<Circle> circles = {
        Circle(Vector2D(0, 0), Vector2D(2, 3), 1.0f, 2.0f),
        Circle(Vector2D(3, 2), Vector2D(-1, -2), 1.0f, 3.0f),
        Circle(Vector2D(-2, -1), Vector2D(1, 1), 1.0f, 1.5f)
    };

    // Run the simulation for 100 steps
    for (int step = 0; step < 100; ++step) {
        cout << "Step " << step + 1 << ":\n";
        simulateStep(circles, deltaTime);

        // Display positions and velocities
        for (size_t i = 0; i < circles.size(); ++i) {
            cout << "Circle " << i + 1 << " -> Position: (" << circles[i].position.x << ", " << circles[i].position.y
                 << "), Velocity: (" << circles[i].velocity.x << ", " << circles[i].velocity.y << ")\n";
        }
        cout << "\n";
    }

    return 0;
}