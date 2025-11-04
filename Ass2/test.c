#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// ---------- Sorting Functions ----------
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

void insertionSortDesc(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] < key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

// ---------- Main ----------
int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter elements: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    int choice;

    while (1) { // infinite loop until user chooses Exit
        printf("\n==============================\n");
        printf("Choose Process Behavior:\n");
        printf("1. Normal Process\n2. Zombie Process\n3. Orphan Process\n4. Exit\n");
        printf("==============================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 4) {
            printf("\nExiting program...\n");
            break;
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }

        if (pid == 0) {
            // ---------- CHILD ----------
            printf("\n[Child] PID = %d | PPID = %d\n", getpid(), getppid());
            printf("[Child] Sorting (Descending): ");
            insertionSortDesc(arr, n);
            printArray(arr, n);

            if (choice == 3) {
                printf("\n[Child] Parent will exit, child sleeping for 10s...\n");
                sleep(10);
                printf("[Child] Woke up. Now orphaned (adopted by init). New PPID = %d\n", getppid());
            }

            printf("\n[Child] Displaying current process info:\n");
            char cmd[128];
            sprintf(cmd, "ps -elf | grep %d | grep -v grep", getpid());
            system(cmd);

            printf("\n[Child] Exiting now.\n");
            exit(0);
        } 
        else {
            // ---------- PARENT ----------
            printf("\n[Parent] PID = %d | Child PID = %d\n", getpid(), pid);
            printf("[Parent] Sorting (Ascending): ");
            bubbleSort(arr, n);
            printArray(arr, n);

            switch (choice) {
                case 1:
                    printf("\n[Parent] Waiting for child (Normal case)...\n");
                    wait(NULL);
                    printf("[Parent] Child finished. Parent exits normally.\n");
                    break;

                case 2:
                    printf("\n[Parent] Not waiting (Zombie case)...\n");
                    printf("[Parent] Sleeping for 15s to show zombie state.\n");

                    printf("\n[Before Sleep] Checking process table (you should see <defunct>):\n");
                    system("ps -elf | grep defunct");

                    sleep(15);

                    printf("\n[After Sleep] Checking again (Zombie should still exist):\n");
                    system("ps -elf | grep defunct");

                    printf("\n[Parent] Exiting now (Zombie will be cleaned automatically after parent exit).\n");
                    break;

                case 3:
                    printf("\n[Parent] Exiting immediately (Orphan case)...\n");
                    exit(0);
                    break;

                default:
                    printf("Invalid choice.\n");
            }
        }
    }
    return 0;
}
