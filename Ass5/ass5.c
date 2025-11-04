#include <stdio.h>
#include <stdbool.h>

int main() {
    int n, m; // n = number of processes, m = number of resources
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resources: ");
    scanf("%d", &m);

    int alloc[n][m], max[n][m], avail[m];
    int need[n][m];
    bool finish[n];
    int safeSeq[n], index = 0;

    // Input Allocation Matrix
    printf("Enter allocation matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);

    // Input Maximum Matrix
    printf("Enter max matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &max[i][j]);

    // Input Available Resources
    printf("Enter available resources:\n");
    for (int i = 0; i < m; i++)
        scanf("%d", &avail[i]);

    // Calculate Need Matrix = Max - Allocation
    printf("\nNeed matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }

    // Initially, no process is finished
    for (int i = 0; i < n; i++)
        finish[i] = false;

    // Find Safe Sequence
    for (int k = 0; k < n; k++) { // repeat until all processes are checked
        for (int i = 0; i < n; i++) {
            if (finish[i] == 0) {
                bool canExecute = true;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > avail[j]) {
                        canExecute = false;
                        break;
                    }
                }

                if (canExecute) {
                    // Add to safe sequence
                    for (int j = 0; j < m; j++)
                        avail[j] += alloc[i][j];

                    safeSeq[index++] = i;
                    finish[i] = true;
                }
            }
        }
    }

    // Check if all processes could finish
    int safe = 1;
    for (int i = 0; i < n; i++)
        if (finish[i] == false)
            safe = 0;

    if (safe) {
        printf("\nSystem is in a SAFE state.\nSafe sequence: ");
        for (int i = 0; i < n; i++)
            printf("P%d ", safeSeq[i]);
    } else {
        printf("\nSystem is in an UNSAFE state (deadlock possible).\n");
    }

    return 0;
}
