#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void insertionSort(int arr[], int n){
    for(int i=1; i<n; i++){
        int key = arr[i];
        int j = i-1;
        while(j >= 0 && arr[j] < key){
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main(){
    int n;
    printf("Enter the number of elements: ");
    scanf("%d", &n);
    int arr[n];
    printf("Enter elements: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);
    int choice;
    
    while(1){
        printf("MENU\n");
        printf("1.Normal Sort \n2.Zombie Process \n3.Orphan Process \n4.Exit \nEnter your choice: ");
        scanf("%d", &choice);
        
        if(choice == 4){
            printf("Exiting...\n");
            break;
        }
        
        pid_t pid = fork();
        if(pid < 0){
            perror("Fork failed");
            exit(1);
        }else if(pid == 0){
            printf("\n[Child] PID = %d | PPID = %d\n", getpid(), getppid());
            printf("[Parent] Sorting (Ascending): ");
            insertionSort(arr, n);
            printArray(arr, n);
            
            if(choice == 3){
                printf("\n[Child]Parent will exit. Child will sleep for 10 sec");
                sleep(10);
                printf("[Child] Woke up. Now orphaned (adopted by init). New PPID = %d\n", getppid());
            }
            
            printf("\n[Child] Display the processes.");
            char cmd[128];
            sprintf(cmd, "ps -elf | grep %d | grep -v grep", getpid());
            system(cmd);
            printf("\nChild exiting now.\n");
            exit(0);
        }else{
            printf("\n[Parent] PID = %d | Child PID = %d\n", getpid(), pid);
            printf("[Parent] Sorting (Ascending): ");
            bubbleSort(arr, n);
            printArray(arr, n);
            
            switch(choice){
                case 1:
                    printf("[Parent] waiting for child\n");
                    wait(NULL);
                    printf("Child finished.\n");
                    break;
                
                case 2:
                    printf("[Parent] Parent sleeps so child can become zombie\n");
                    system("ps -elf | grep defunct");
                    sleep(15);
                    printf("[Parent] You can see the zombie state\n");
                    system("ps -elf | grep defunct");
                    break;
                
                case 3:
                    printf("Parent exits immediately\n");
                    exit(0);
                    break;
                
                default:
                    printf("Invalid choice.\n");
            }
        }
    }
    return 0;
}