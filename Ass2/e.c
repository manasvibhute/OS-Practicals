#include <stdio.h>
#include <stdlib.h>

int binarySearch(int arr[], int n, int target){
    int low = 0, high = n - 1;
    while(low <= high){
        int mid = low + (high - low) / 2;
        if(arr[mid] < target)
            low = mid + 1;
        else if(arr[mid] > target)
            high = mid - 1;
        else
            return mid;
    }
    return -1;
}

int main(int argc, char *argv[]){
    printf("This is the child program\n");

    if(argc <= 1){
        printf("Usage: %s <numbers>\n", argv[0]);
        return 1;
    }

    int arr[argc - 1];
    for(int i = 1; i < argc; i++)
        arr[i - 1] = atoi(argv[i]); // convert arguments to integers

    int t;
    printf("Tell the target: ");
    scanf("%d", &t);

    int result = binarySearch(arr, argc - 1, t);
    if(result != -1)
        printf("Element %d found at index %d.\n", t, result);
    else
        printf("Element %d not found in array.\n", t);

    return 0;
}
