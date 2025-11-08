#include <stdio.h>
#include <stdbool.h>

#define MAXP 10
#define MAXR 10

int main()
{
    int n, m; // number of processes and resources
    int alloc[MAXP][MAXR], max[MAXP][MAXR], need[MAXP][MAXR], avail[MAXR];
    int safeSeq[MAXP], count = 0;
    bool finish[MAXP] = {false};

    FILE *file = fopen("state.txt", "r");
    if (!file)
    {
        perror("Error opening state.txt");
        return 1;
    }

    // ---------- Read data from file ----------
    if (fscanf(file, "%d %d", &n, &m) != 2)
    {
        printf("Invalid or empty state.txt file\n");
        fclose(file);
        return 1;
    }

    printf("Number of Processes: %d\n", n);
    printf("Number of Resources: %d\n", m);

    printf("\n--- Allocation Matrix ---\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            fscanf(file, "%d", &alloc[i][j]);
            printf("%d ", alloc[i][j]);
        }
        printf("\n");
    }

    printf("\n--- Max Matrix ---\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            fscanf(file, "%d", &max[i][j]);
            printf("%d ", max[i][j]);
        }
        printf("\n");
    }

    printf("\n--- Available Resources ---\n");
    for (int i = 0; i < m; i++)
    {
        fscanf(file, "%d", &avail[i]);
        printf("%d ", avail[i]);
    }
    printf("\n");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (max[i][j] < alloc[i][j])
            {
                printf("\n❌ Invalid Input: For Process %d, Resource %d -> Max (%d) < Allocation (%d)\n",
                       i, j, max[i][j], alloc[i][j]);
                printf("System state invalid. Please fix 'state.txt'.\n");
                return 1;
            }
        }
    }

    fclose(file);

    // ---------- Calculate Need Matrix ----------
    printf("\n--- Need Matrix ---\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            need[i][j] = max[i][j] - alloc[i][j];
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }

    // ---------- Safety Algorithm ----------
    printf("\nChecking system safety...\n");
    for (int i = 0; i < n; i++)
        finish[i] = false;

    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            if (!finish[i])
            {
                bool canExecute = true;
                for (int j = 0; j < m; j++)
                {
                    if (need[i][j] > avail[j])
                    {
                        canExecute = false;
                        break;
                    }
                }

                if (canExecute)
                {
                    for (int j = 0; j < m; j++)
                        avail[j] += alloc[i][j];
                    safeSeq[count++] = i;
                    finish[i] = true;
                }
            }
        }
    }

    bool safe = true;
    for (int i = 0; i < n; i++)
        if (!finish[i])
            safe = false;

    if (safe)
    {
        printf("\n✅ System is in a SAFE state.\nSafe sequence: ");
        for (int i = 0; i < n; i++)
            printf("P%d ", safeSeq[i]);
        printf("\n");
    }
    else
    {
        printf("\n❌ System is in an UNSAFE state (deadlock possible).\n");
        return 0;
    }

    // ---------- Resource Request ----------
    int reqProc, request[MAXR];
    printf("\nEnter process number making request (0-%d): ", n - 1);
    scanf("%d", &reqProc);

    printf("Enter request vector (%d values): ", m);
    for (int i = 0; i < m; i++)
        scanf("%d", &request[i]);

    bool grant = true;
    for (int i = 0; i < m; i++)
    {
        if (request[i] > need[reqProc][i])
        {
            printf("Error: Process has exceeded its maximum claim!\n");
            grant = false;
            break;
        }
        if (request[i] > avail[i])
        {
            printf("Resources not available now. Request cannot be granted.\n");
            grant = false;
            break;
        }
    }

    if (grant)
    {
        // Temporarily allocate
        for (int i = 0; i < m; i++)
        {
            avail[i] -= request[i];
            alloc[reqProc][i] += request[i];
            need[reqProc][i] -= request[i];
        }

        // Recheck safety
        for (int i = 0; i < n; i++)
            finish[i] = false;
        count = 0;
        bool newSafe = true;

        for (int k = 0; k < n; k++)
        {
            for (int i = 0; i < n; i++)
            {
                if (!finish[i])
                {
                    bool canExecute = true;
                    for (int j = 0; j < m; j++)
                    {
                        if (need[i][j] > avail[j])
                        {
                            canExecute = false;
                            break;
                        }
                    }
                    if (canExecute)
                    {
                        for (int j = 0; j < m; j++)
                            avail[j] += alloc[i][j];
                        safeSeq[count++] = i;
                        finish[i] = true;
                    }
                }
            }
        }

        for (int i = 0; i < n; i++)
            if (!finish[i])
                newSafe = false;

        if (newSafe)
        {
            printf("\n✅ Request can be GRANTED. System remains SAFE.\nSafe sequence: ");
            for (int i = 0; i < n; i++)
                printf("P%d ", safeSeq[i]);
            printf("\n");
        }
        else
        {
            printf("\n❌ Request CANNOT be granted. System would be UNSAFE.\n");
        }
    }

    return 0;
}
