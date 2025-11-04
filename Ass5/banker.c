#include <stdio.h>
#include <stdbool.h>

// Function prototypes
bool isSafe(int n, int m, int alloc[n][m], int need[n][m], int avail[m], int safeSeq[n]);
bool requestResource(int n, int m, int alloc[n][m], int need[n][m], int avail[m], int reqProcess, int request[m], int safeSeq[n]);

// SAFETY ALGORITHM: Checks if the system is in a safe state
bool isSafe(int n, int m, int alloc[n][m], int need[n][m], int avail[m], int safeSeq[n]) {
    bool finish[n];   // Tracks whether each process can finish
    int work[m];      // Temporary copy of available resources

    // Initialize work = available
    for (int i = 0; i < m; i++)
        work[i] = avail[i];

    // Initially, no process is finished
    for (int i = 0; i < n; i++)
        finish[i] = false;

    int count = 0; // Count of finished processes
    while (count < n) {
        bool found = false; // True if at least one process can finish in this loop

        for (int i = 0; i < n; i++) {
            if (!finish[i]) { // Only consider unfinished processes
                bool canAllocate = true;

                // Check if process i's needs can be satisfied with current work
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }

                // If yes, assume it finishes and release its resources
                if (canAllocate) {
                    for (int j = 0; j < m; j++)
                        work[j] += alloc[i][j]; // Release allocated resources
                    safeSeq[count++] = i; // Add process to safe sequence
                    finish[i] = true;
                    found = true;
                }
            }
        }

        // If no process could be allocated safely, system is unsafe
        if (!found)
            return false;
    }

    // All processes can finish → system is safe
    return true;
}

// RESOURCE REQUEST ALGORITHM
bool requestResource(int n, int m, int alloc[n][m], int need[n][m], int avail[m],
                     int reqProcess, int request[m], int safeSeq[n]) {
    // Step 1: Check if request <= need
    for (int i = 0; i < m; i++) {
        if (request[i] > need[reqProcess][i]) {
            printf("Error: Process has exceeded its maximum claim.\n");
            return false;
        }
    }

    // Step 2: Check if request <= available
    for (int i = 0; i < m; i++) {
        if (request[i] > avail[i]) {
            printf("Resources not available. Process must wait.\n");
            return false;
        }
    }

    // Step 3: Pretend allocation (temporary state)
    for (int i = 0; i < m; i++) {
        avail[i] -= request[i];           // Reduce available resources
        alloc[reqProcess][i] += request[i]; // Allocate to the process
        need[reqProcess][i] -= request[i];  // Reduce the process’s remaining need
    }

    // Step 4: Check if this new state is safe
    if (isSafe(n, m, alloc, need, avail, safeSeq)) {
        printf("\nAfter allocation, system is in a safe state.\nSafe sequence: ");
        for (int i = 0; i < n; i++)
            printf("%d ", safeSeq[i]);
        printf("\n");
        return true; // Allocation is safe
    } else {
        // Step 5: Rollback (undo allocation)
        for (int i = 0; i < m; i++) {
            avail[i] += request[i];
            alloc[reqProcess][i] -= request[i];
            need[reqProcess][i] += request[i];
        }
        printf("\nAfter allocation, system would NOT be safe. Request denied.\n");
        return false;
    }
}

// --------------------------------------------------
// MAIN FUNCTION
// --------------------------------------------------
int main() {
    int n, m;

    // Step 1: Input number of processes and resource types
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resource types: ");
    scanf("%d", &m);

    int alloc[n][m], max[n][m], avail[m];
    int need[n][m], safeSeq[n];

    // Step 2: Input Allocation Matrix
    printf("\nEnter Allocation Matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);

    // Step 3: Input Maximum Matrix
    printf("\nEnter Max Matrix (%d x %d):\n", n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &max[i][j]);

    // Step 4: Input Available Resources
    printf("\nEnter Available Resources (%d):\n", m);
    for (int i = 0; i < m; i++)
        scanf("%d", &avail[i]);

    // Step 5: Calculate Need Matrix = Max - Allocation
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];

    // Step 6: Check initial system safety
    if (isSafe(n, m, alloc, need, avail, safeSeq)) {
        printf("\nSystem is initially in a safe state.\nSafe sequence: ");
        for (int i = 0; i < n; i++)
            printf("%d ", safeSeq[i]);
        printf("\n");
    } else {
        printf("\nSystem is NOT in a safe state initially!\n");
        return 0;
    }

    // Step 7: Take a resource request from user
    int reqProcess, request[m];
    printf("\nEnter the process number making a request (0 to %d): ", n - 1);
    scanf("%d", &reqProcess);
    printf("Enter the request for each resource type:\n");
    for (int i = 0; i < m; i++)
        scanf("%d", &request[i]);

    // Step 8: Process the request
    requestResource(n, m, alloc, need, avail, reqProcess, request, safeSeq);

    return 0;
}
