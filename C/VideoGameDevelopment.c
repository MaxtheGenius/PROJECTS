#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure for a 3D vector (for position, velocity, etc.)
typedef struct
{
    float x, y, z;
} Vector3;

// Structure for a 3D axis-aligned bounding box (AABB)
typedef struct
{
    Vector3 min; // Minimum corner of the AABB (e.g., bottom-left-back)
    Vector3 max; // Maximum corner of the AABB (e.g., top-right-front)
} AABB;

// Function to check if two AABBs are colliding
bool checkCollision(AABB box1, AABB box2)
{
    return (box1.max.x > box2.min.x && box1.min.x < box2.max.x) &&
           (box1.max.y > box2.min.y && box1.min.y < box2.max.y) &&
           (box1.max.z > box2.min.z && box1.min.z < box2.max.z);
}

// Function to resolve collision by adjusting position
void resolveCollision(AABB *box1, AABB *box2)
{
    // Simple collision resolution by moving box1 out of the collision
    if (checkCollision(*box1, *box2))
    {
        float overlapX = 0.0f;
        float overlapY = 0.0f;
        float overlapZ = 0.0f;

        // Check how much the boxes overlap in each direction
        if (box1->max.x > box2->min.x && box1->min.x < box2->max.x)
        {
            overlapX = fminf(box1->max.x - box2->min.x, box2->max.x - box1->min.x);
        }
        if (box1->max.y > box2->min.y && box1->min.y < box2->max.y)
        {
            overlapY = fminf(box1->max.y - box2->min.y, box2->max.y - box1->min.y);
        }
        if (box1->max.z > box2->min.z && box1->min.z < box2->max.z)
        {
            overlapZ = fminf(box1->max.z - box2->min.z, box2->max.z - box1->min.z);
        }

        // Resolve collision by adjusting position (simple response)
        if (overlapX > overlapY && overlapX > overlapZ)
        {
            box1->min.x -= overlapX;
            box1->max.x -= overlapX;
        }
        else if (overlapY > overlapX && overlapY > overlapZ)
        {
            box1->min.y -= overlapY;
            box1->max.y -= overlapY;
        }
        else
        {
            box1->min.z -= overlapZ;
            box1->max.z -= overlapZ;
        }
    }
}

// Function to simulate physics (gravity, movement)
void updatePosition(AABB *box, Vector3 velocity, float deltaTime)
{
    box->min.x += velocity.x * deltaTime;
    box->min.y += velocity.y * deltaTime;
    box->min.z += velocity.z * deltaTime;

    box->max.x += velocity.x * deltaTime;
    box->max.y += velocity.y * deltaTime;
    box->max.z += velocity.z * deltaTime;
}

// Function to print AABB details
void printAABB(AABB box)
{
    printf("Min: (%.2f, %.2f, %.2f), Max: (%.2f, %.2f, %.2f)\n",
           box.min.x, box.min.y, box.min.z, box.max.x, box.max.y, box.max.z);
}

int main()
{
    // Define two AABBs (player and platform)
    AABB player = {{0.0f, 0.0f, 0.0f}, {1.0f, 2.0f, 1.0f}};
    AABB platform = {{0.0f, -1.0f, 0.0f}, {5.0f, 0.0f, 5.0f}};

    // Define player velocity
    Vector3 playerVelocity = {0.0f, -9.8f, 0.0f}; // Gravity effect, falling

    // Time step for physics update (simulation)
    float deltaTime = 0.016f; // 60 FPS (1/60)

    // Simulate the movement and collision check
    for (int i = 0; i < 100; i++)
    { // Run the simulation for 100 frames
        printf("\nFrame %d:\n", i + 1);

        // Update player position based on velocity
        updatePosition(&player, playerVelocity, deltaTime);

        // Check for collision and resolve if necessary
        if (checkCollision(player, platform))
        {
            printf("Collision detected!\n");
            resolveCollision(&player, &platform);
        }
        else
        {
            printf("No collision.\n");
        }

        // Print the player's new position
        printAABB(player);
    }

    return 0;
}