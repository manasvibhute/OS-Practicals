#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

void merge(int arr[], int left, int mid, int right)
{
    int i = left, j = mid + 1, k = 0;
    int temp[right - left + 1];

    while (i <= mid && j <= right)
    {
        if (arr[i] < arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    while (i <= mid)
        temp[k++] = arr[i++];
    while (j <= right)
        temp[k++] = arr[j++];

    // copy back to arr[left..right]
    for (int p = 0; p < k; p++) 
        arr[left + p] = temp[p];
}

void mergeSort(int arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

int main()
{
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);
    int arr[n];
    printf("Enter %d elements: ", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    // Parent sorts using merge sort
    mergeSort(arr, 0, n - 1);

    printf("Sorted Array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
    
    pid_t pid = fork();
    if(pid < 0){
        perror("Fork Failed");
        exit(1);
    }else if(pid == 0){
        char *args[n+2];
        args[0] = "./exec";
        for(int i=0; i<n; i++){
            char *buf = malloc(20);
            snprintf(buf, 20, "%d", arr[i]);
            args[i+1] = buf;
        }
        args[n+1] = NULL;
        
        execve("./exec", args, NULL);
        perror("Child Failed");
        exit(1);
    }else{
        wait(NULL);
    }
    return 0;
}