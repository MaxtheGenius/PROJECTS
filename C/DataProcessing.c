#include <stdio.h>
#include <stdlib.h>

#define MAX_DATA_POINTS 10000 // Maximum number of data points we can handle

// Function prototypes
void readData(const char *filename, float data[], int *dataCount);
float calculateAverage(const float data[], int dataCount);
float findMaxTemperature(const float data[], int dataCount);
float findMinTemperature(const float data[], int dataCount);

int main()
{
    const char *filename = "temperatures.txt"; // Dataset file name
    float data[MAX_DATA_POINTS];               // Array to store temperatures
    int dataCount = 0;                         // To keep track of the number of data points

    // Read data from file
    readData(filename, data, &dataCount);

    if (dataCount == 0)
    {
        printf("No data to process.\n");
        return 1;
    }

    // Calculate statistics
    float average = calculateAverage(data, dataCount);
    float maxTemp = findMaxTemperature(data, dataCount);
    float minTemp = findMinTemperature(data, dataCount);

    // Display results
    printf("Temperature Data Analysis:\n");
    printf("Average Temperature: %.2f\n", average);
    printf("Maximum Temperature: %.2f\n", maxTemp);
    printf("Minimum Temperature: %.2f\n", minTemp);

    return 0;
}

// Function to read data from a file
void readData(const char *filename, float data[], int *dataCount)
{
    FILE *file = fopen(filename, "r"); // Open the file in read mode
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Read each line of the file and store the temperature values
    while (fscanf(file, "%f", &data[*dataCount]) == 1)
    {
        (*dataCount)++;
        if (*dataCount >= MAX_DATA_POINTS)
        {
            printf("Reached the maximum data points limit.\n");
            break;
        }
    }

    fclose(file); // Close the file after reading
}

// Function to calculate the average temperature
float calculateAverage(const float data[], int dataCount)
{
    float sum = 0.0;
    for (int i = 0; i < dataCount; i++)
    {
        sum += data[i];
    }
    return sum / dataCount;
}

// Function to find the maximum temperature
float findMaxTemperature(const float data[], int dataCount)
{
    float maxTemp = data[0];
    for (int i = 1; i < dataCount; i++)
    {
        if (data[i] > maxTemp)
        {
            maxTemp = data[i];
        }
    }
    return maxTemp;
}

// Function to find the minimum temperature
float findMinTemperature(const float data[], int dataCount)
{
    float minTemp = data[0];
    for (int i = 1; i < dataCount; i++)
    {
        if (data[i] < minTemp)
        {
            minTemp = data[i];
        }
    }
    return minTemp;
}