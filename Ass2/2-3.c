#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

int isPrime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return 0;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive integer>\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]); //argv[1] is a string like "10" and atoi() converts that string into an integer → here n = 10.
    if (n <= 0) {
        fprintf(stderr, "Error: Enter a positive integer.\n");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } 
    else if (pid == 0) {
        // ---- Child process ----
        printf("Child (PID %d): First %d prime numbers:\n", getpid(), n);
        int count = 0, num = 2;
        while (count < n) {
            if (isPrime(num)) {
                printf("%d ", num);
                count++;
            }
            num++;
        }
        printf("\nChild exiting.\n");
        exit(0);
    } 
    else {
        // ---- Parent process ----
        printf("Parent (PID %d): Waiting for child...\n", getpid());
        wait(NULL);
        printf("Parent: Child completed. Exiting.\n");
    }

    return 0;
}
