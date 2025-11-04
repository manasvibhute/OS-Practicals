#include <stdio.h>
#include <stdlib.h>
// Function to reverse sort an array (without using qsort)
void reverseSort(int arr[], int n)
{
    // Simple bubble sort in descending order
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] < arr[j + 1])
            {
                // Swap elements
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}
// Binary Search Function for descending order
int binarySearchDesc(int arr[], int n, int target)
{
    int low = 0, high = n - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (arr[mid] == target)
            return mid; // Target found
        if (arr[mid] < target)
            high = mid - 1; // Search left half (since array is descending)
        else
            low = mid + 1; // Search right half
    }
    return -1; // Target not found
}

int main(int argc, char *argv[])
/*argc → argument count (number of command-line arguments including program name)
argv → argument values (array of strings)*/
{
    printf("This is the child file\n");
    if (argc <= 1) //If no numbers were passed, print a usage message and exit.
    {
        printf("usage: %s <numbers>\n", argv[0]); //argv[0] is the program name-(eg)./a.out
        return 1;
    }

    int arr[argc - 1];
    for (int i = 1; i < argc; i++)
    {
        arr[i - 1] = atoi(argv[i]); //converts them to integers
    }

    reverseSort(arr, argc - 1);

    printf("Array in descending order: ");
    for (int i = 0; i < argc - 1; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    int target;
    printf("Enter value to search: ");
    scanf("%d", &target);
    // Call the binary search function
    int result = binarySearchDesc(arr, argc - 1, target);
    // Output the result of the search
    if (result != -1)
    {
        printf("Element %d found at index %d.\n", target, result);
    }
    else
    {
        printf("Element %d not found.\n", target);
    }
    return 0;
}