#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // swap
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main(){
    int arr[100], n, i;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    printf("Enter %d elements:\n", n);
    for (i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    bubbleSort(arr, n);

    printf("\nSorted array (ascending order):\n");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    pid_t pid = fork();
    if(pid < 0){
        perror("Fork failed");
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
        perror("Fork failed");
        exit(1);
    }else{
        printf("Wait for child to finish");
        wait(NULL);
    }
    return 0;
}