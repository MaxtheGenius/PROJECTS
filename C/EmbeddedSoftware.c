#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

// Global variables
volatile int currentTemperature = 20; // Initial temperature
volatile int targetTemperature = 22;  // Desired temperature
volatile int heaterOn = 0;            // Heater status (0 = OFF, 1 = ON)

// Signal handler for controlling temperature
void temperatureControlHandler(int signum)
{
    (void)signum; // Suppress unused parameter warning
    if (currentTemperature < targetTemperature)
    {
        heaterOn = 1;
        printf("Heater ON: Current temperature: %d°C, Target: %d°C\n",
               currentTemperature, targetTemperature);
        currentTemperature++; // Simulate heating
    }
    else if (currentTemperature > targetTemperature)
    {
        heaterOn = 0;
        printf("Heater OFF: Current temperature: %d°C, Target: %d°C\n",
               currentTemperature, targetTemperature);
        currentTemperature--; // Simulate cooling
    }
    else
    {
        heaterOn = 0;
        printf("Temperature stable at %d°C. Heater OFF.\n", currentTemperature);
    }
}

// Set up a periodic timer to generate signals
void setupTimer(int interval)
{
    struct sigaction sa;
    struct itimerval timer;

    // Configure the signal handler
    sa.sa_handler = &temperatureControlHandler;
    sa.sa_flags = SA_RESTART; // Restart interrupted system calls
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
    printf("Smart Thermostat Control System\n");
    printf("Current temperature: %d°C\n", currentTemperature);
    printf("Target temperature: %d°C\n", targetTemperature);

    // Set up the periodic timer to control temperature
    setupTimer(1);

    // User input loop for adjusting the target temperature
    while (1)
    {
        int newTarget;
        printf("\nEnter new target temperature (or -1 to exit): ");
        if (scanf("%d", &newTarget) != 1)
        {
            printf("Invalid input. Exiting.\n");
            break;
        }

        if (newTarget == -1)
        {
            printf("Exiting the thermostat control system.\n");
            break;
        }

        targetTemperature = newTarget;
        printf("Updated target temperature to %d°C.\n", targetTemperature);
    }

    return 0;
}