#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

void merge(int arr[], int left, int right, int mid)
{
    int i = left;
    int j = mid + 1;
    int k = 0;
    int temp[right - left + 1];

    while (i <= mid && j <= right)
    {
        if (arr[i] < arr[j])
        {
            temp[k++] = arr[i++];
        }
        else
        {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid)
    {
        temp[k++] = arr[i++];
    }
    while (j <= right)
    {
        temp[k++] = arr[j++];
    }

    k=0;
    for (int i = 0, k = 0; i <= right; i++, k++)
    {
        arr[i] = temp[k];
    }
}

void mergeSort(int arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, right, mid);
    }
}

int main()
{
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);
    int arr[n];
    printf("Enter %d elements : ", n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    mergeSort(arr, 0, n - 1);

    printf("Sorted Array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    pid_t pid = fork();
    if(pid < 0){
        perror("Fork Failed");
        exit(1);
    }
    else if (pid == 0){
        char *args[n+2]; //This creates an array of strings (character pointers) — the arguments that we will pass to a new program when we execute it using execve.
        args[0] = "./exec"; //an executable file named exec in the current directory.
        
        for(int i=0; i<n; i++){
            char *buf = malloc(20); //dynamically allocating 20 bytes of memory to hold a string.
            snprintf(buf, 20, "%d", arr[i]); //convert
            args[i+1] = buf; //put into the arg array
        }
        args[n+1] = NULL;
        
        execve("./exec", args, NULL);
        /*This replaces the current process (child) with a new program (./exec).
        The child process stops running this current code.
        It loads and runs the code from the new program (exec).
        The new program receives args as its command-line arguments.
        If execve succeeds, this line never returns — the old code is replaced. */
        perror("execve failed"); 
        exit(1);
    }else{ //pid > 0, this is the parent process.
        wait(NULL);
    }
    
    return 0;
}