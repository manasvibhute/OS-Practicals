#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void bubble(int a[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j] > a[j + 1]) {
                int t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }
}

void insertion(int a[], int n) {
    for (int i = 1; i < n; i++) {
        int key = a[i], j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

void print(int a[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");
}

int main() {
    int n, a[100];
    printf("Enter n: ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);

    pid_t pid = fork();

    if (pid == 0) {   // Child process
        insertion(a, n);
        printf("[Child] Insertion Sort: ");
        print(a, n);  
        printf("\n[Child] PID=%d PPID=%d\n", getpid(), getppid());
        //sleep(10);  // Observe zombie/orphan behavior
        printf("\n[Child] PID=%d PPID=%d\n", getpid(), getppid());
        printf("[Child] Exiting\n");
        // exit(0);  // Uncomment if needed
    } 
    else if (pid > 0) {  // Parent process
        // wait(NULL);  // Uncomment this to remove zombie
        printf("[Parent] Bubble Sort: ");
        bubble(a, n);
        print(a, n);
        printf("\n[Parent] PID=%d ChildPID=%d\n", getpid(), pid);
        sleep(10);
        //_exit(0);  // Parent exits early → child becomes orphan
    } 
    else {
        perror("Fork failed");
        exit(1);
    }

    return 0;
}