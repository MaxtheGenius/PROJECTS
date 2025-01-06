#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

// Enum to define traffic light states
typedef enum
{
    RED,
    GREEN,
    YELLOW
} LightState;

// Traffic light structure
typedef struct
{
    LightState state; // Current state of the light
    int timer;        // Time remaining in the current state
} TrafficLight;

// Global variables for the traffic lights
TrafficLight northSouth = {RED, 10};
TrafficLight eastWest = {GREEN, 15};

// Signal handler to update traffic light states
void updateTrafficLights(int signum)
{
    (void)signum; // Suppress unused parameter warning

    // Update North-South traffic light
    if (northSouth.timer > 0)
    {
        northSouth.timer--;
    }
    else
    {
        if (northSouth.state == RED)
        {
            northSouth.state = GREEN;
            northSouth.timer = 15;
        }
        else if (northSouth.state == GREEN)
        {
            northSouth.state = YELLOW;
            northSouth.timer = 3;
        }
        else if (northSouth.state == YELLOW)
        {
            northSouth.state = RED;
            northSouth.timer = 10;
        }
    }

    // Update East-West traffic light
    if (eastWest.timer > 0)
    {
        eastWest.timer--;
    }
    else
    {
        if (eastWest.state == RED)
        {
            eastWest.state = GREEN;
            eastWest.timer = 15;
        }
        else if (eastWest.state == GREEN)
        {
            eastWest.state = YELLOW;
            eastWest.timer = 3;
        }
        else if (eastWest.state == YELLOW)
        {
            eastWest.state = RED;
            eastWest.timer = 10;
        }
    }

    // Display current state
    printf("\nNorth-South: %s (%d seconds remaining)\n",
           northSouth.state == RED ? "RED" : northSouth.state == GREEN ? "GREEN"
                                                                       : "YELLOW",
           northSouth.timer);
    printf("East-West: %s (%d seconds remaining)\n",
           eastWest.state == RED ? "RED" : eastWest.state == GREEN ? "GREEN"
                                                                   : "YELLOW",
           eastWest.timer);
}

// Set up the timer for periodic updates
void setupTimer(int interval)
{
    struct sigaction sa;
    struct itimerval timer;

    // Configure the signal handler
    sa.sa_handler = updateTrafficLights; // Directly use the function name
    sa.sa_flags = 0;                     // Avoid SA_RESTART if causing issues
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGALRM, &sa, NULL) == -1)
    {
        perror("Error setting up signal handler");
        exit(EXIT_FAILURE);
    }

    // Configure the timer to expire every 'interval' seconds
    timer.it_value.tv_sec = interval;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = interval;
    timer.it_interval.tv_usec = 0;

    // Start the timer
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
    {
        perror("Error setting up timer");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    printf("Traffic Control System Initialized.\n");
    printf("North-South starts with RED, East-West starts with GREEN.\n");

    // Set up periodic timer to update traffic lights
    setupTimer(1);

    // Keep the program running to simulate the traffic lights
    while (1)
    {
        pause(); // Wait for signals
    }

    return 0;
}