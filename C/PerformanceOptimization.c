#include <stdio.h>
#include <stdlib.h>

// Threshold for switching to Insertion Sort
#define INSERTION_SORT_THRESHOLD 10

// Insertion Sort function for small subarrays
void insertionSort(int arr[], int low, int high)
{
    for (int i = low + 1; i <= high; i++)
    {
        int key = arr[i];
        int j = i - 1;

        // Move elements greater than 'key' to one position ahead
        while (j >= low && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Partition function for QuickSort
int partition(int arr[], int low, int high)
{
    int pivot = arr[high]; // Take the last element as the pivot
    int i = low - 1;

    // Rearrange elements to have smaller ones on the left and larger ones on the right
    for (int j = low; j < high; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            // Swap arr[i] and arr[j]
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    // Place the pivot in its correct position
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

// Optimized QuickSort function
void quickSort(int arr[], int low, int high)
{
    while (low < high)
    {
        // Use Insertion Sort for small subarrays
        if (high - low + 1 <= INSERTION_SORT_THRESHOLD)
        {
            insertionSort(arr, low, high);
            break;
        }
        else
        {
            // Partition the array
            int pivot = partition(arr, low, high);

            // Sort the smaller subarray first (tail recursion optimization)
            if (pivot - low < high - pivot)
            {
                quickSort(arr, low, pivot - 1);
                low = pivot + 1; // Update low for the next iteration
            }
            else
            {
                quickSort(arr, pivot + 1, high);
                high = pivot - 1; // Update high for the next iteration
            }
        }
    }
}

// Main function to test the optimized sorting algorithm
int main()
{
    int arr[] = {24, 97, 40, 67, 88, 85, 15, 92, 75, 65, 43, 52, 35};
    int n = sizeof(arr) / sizeof(arr[0]);

    // Print the original array
    printf("Original array:\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Sort the array
    quickSort(arr, 0, n - 1);

    // Print the sorted array
    printf("Sorted array:\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}