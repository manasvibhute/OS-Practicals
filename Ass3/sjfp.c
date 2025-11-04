// sjf_preemptive.c
#include <stdio.h>
#include <limits.h>

struct Process {
    int pid, at, bt, ct, tat, wt;
};

struct ExecSegment {
    int pid, start, end;
};

int sjfPreemptive(struct Process p[], int n, struct ExecSegment gantt[], int execOrder[], int *execCount){
    int rem[n];
    for (int i = 0; i < n; i++) rem[i] = p[i].bt;
    int time = 0, s = 0, completed = 0, lastPid = -1;
    *execCount = 0;

    while (completed < n){
        int idx = -1, minBT = INT_MAX;
        for (int i = 0; i < n; i++){
            if (p[i].at <= time && rem[i] > 0 && rem[i] < minBT){
                minBT = rem[i];
                idx = i;
            }
        }
        if (idx == -1){
            time++;
            continue;
        }

        if (lastPid != p[idx].pid){
            gantt[s].pid = p[idx].pid;
            gantt[s].start = time;
            s++;
            lastPid = p[idx].pid;
        }

        // run for 1 time unit
        time++;
        rem[idx]--;
        // record each time unit execution into execOrder (ready queue-like sequence)
        execOrder[(*execCount)++] = p[idx].pid;
        gantt[s - 1].end = time;

        if (rem[idx] == 0){
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt  = p[idx].tat - p[idx].bt;
            completed++;
        }
    }
    return s;
}

void sortByArrival(struct Process p[], int n){
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (p[j].at > p[j+1].at){
                struct Process tmp = p[j];
                p[j] = p[j+1];
                p[j+1] = tmp;
            }
}

void printGantt(struct ExecSegment gantt[], int segCount){
    printf("\nGantt Chart:\n ");
    for (int i = 0; i < segCount; i++) printf("------"); printf("-\n|");
    for (int i = 0; i < segCount; i++) printf(" P%-2d |", gantt[i].pid); printf("\n ");
    for (int i = 0; i < segCount; i++) printf("------"); printf("-\n");
    for (int i = 0; i < segCount; i++) printf("%-6d", gantt[i].start);
    if (segCount > 0) printf("%d\n", gantt[segCount - 1].end);
}

void printReadyQueue(int execOrder[], int execCount){
    printf("\nReady Queue (time-slice execution order): ");
    for (int i = 0; i < execCount; i++){
        printf("P%d%s", execOrder[i], (i != execCount - 1) ? " | " : "");
    }
    printf("\n");
}

void printTable(struct Process p[], int n){
    float totalWT = 0, totalTAT = 0;
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++){
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
        totalWT += p[i].wt;
        totalTAT += p[i].tat;
    }
    printf("\nAverage TAT: %.2f\nAverage WT: %.2f\n", totalTAT / n, totalWT / n);
}

int main(){
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    struct Process p[n];
    for (int i = 0; i < n; i++){
        printf("Enter PID, Arrival Time, Burst Time for process %d: ", i+1);
        scanf("%d %d %d", &p[i].pid, &p[i].at, &p[i].bt);
        p[i].ct = p[i].tat = p[i].wt = 0;
    }

    sortByArrival(p, n);

    struct ExecSegment gantt[2000];
    int execOrder[10000], execCount = 0;
    int segCount = sjfPreemptive(p, n, gantt, execOrder, &execCount);

    printGantt(gantt, segCount);
    printReadyQueue(execOrder, execCount);
    printTable(p, n);

    return 0;
}
