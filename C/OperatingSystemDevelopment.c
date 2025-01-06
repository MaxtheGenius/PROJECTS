#include <stdio.h>
#include <stdlib.h>

// Define constants for queue size and maximum processes
#define MAX_PROCESSES 10

// Define the process structure
typedef struct
{
    int id;            // Process ID
    int burstTime;     // Total time required
    int remainingTime; // Time left to execute
} Process;

// Define the queue structure for process management
typedef struct
{
    Process processes[MAX_PROCESSES];
    int front;
    int rear;
    int size;
} ProcessQueue;

// Initialize the queue
void initializeQueue(ProcessQueue *queue)
{
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
}

// Check if the queue is empty
int isQueueEmpty(ProcessQueue *queue)
{
    return queue->size == 0;
}

// Enqueue a process
void enqueue(ProcessQueue *queue, Process process)
{
    if (queue->size == MAX_PROCESSES)
    {
        printf("Queue is full! Cannot add process.\n");
        return;
    }
    queue->rear = (queue->rear + 1) % MAX_PROCESSES;
    queue->processes[queue->rear] = process;
    queue->size++;
}

// Dequeue a process
Process dequeue(ProcessQueue *queue)
{
    if (isQueueEmpty(queue))
    {
        printf("Queue is empty! Exiting.\n");
        exit(EXIT_FAILURE);
    }
    Process process = queue->processes[queue->front];
    queue->front = (queue->front + 1) % MAX_PROCESSES;
    queue->size--;
    return process;
}

// Simulate Round Robin scheduling
void roundRobin(ProcessQueue *queue, int timeQuantum)
{
    int currentTime = 0;

    printf("\nStarting Round Robin Scheduling (Time Quantum: %d):\n", timeQuantum);

    while (!isQueueEmpty(queue))
    {
        Process current = dequeue(queue);

        if (current.remainingTime > timeQuantum)
        {
            currentTime += timeQuantum;
            current.remainingTime -= timeQuantum;
            printf("Time %d: Process %d executed. Remaining time: %d\n",
                   currentTime, current.id, current.remainingTime);
            enqueue(queue, current);
        }
        else
        {
            currentTime += current.remainingTime;
            printf("Time %d: Process %d completed.\n", currentTime, current.id);
        }
    }

    printf("All processes have been completed.\n");
}

// Main function
int main()
{
    ProcessQueue queue;
    initializeQueue(&queue);

    int numProcesses, timeQuantum;

    // Input number of processes
    printf("Enter the number of processes (1-%d): ", MAX_PROCESSES);
    scanf("%d", &numProcesses);

    if (numProcesses <= 0 || numProcesses > MAX_PROCESSES)
    {
        printf("Invalid number of processes. Exiting.\n");
        return EXIT_FAILURE;
    }

    // Input process details
    for (int i = 0; i < numProcesses; i++)
    {
        Process process;
        process.id = i + 1;
        printf("Enter burst time for Process %d: ", process.id);
        scanf("%d", &process.burstTime);
        process.remainingTime = process.burstTime;
        enqueue(&queue, process);
    }

    // Input time quantum
    printf("Enter the time quantum: ");
    scanf("%d", &timeQuantum);

    if (timeQuantum <= 0)
    {
        printf("Invalid time quantum. Exiting.\n");
        return EXIT_FAILURE;
    }

    // Perform Round Robin scheduling
    roundRobin(&queue, timeQuantum);

    return EXIT_SUCCESS;
}