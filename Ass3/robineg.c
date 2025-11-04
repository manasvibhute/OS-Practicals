#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    int remaining; // for RR
};

struct ExecSegment {
    int pid;
    int start;
    int end;
};

// Function prototypes
void inputProcesses(struct Process p[], int n);
void bubbleSortByArrival(struct Process p[], int n);
void printGanttChart(struct ExecSegment gantt[], int segCount);
void printProcessTable(struct Process p[], int n);
void printReadyQueueInOneLine(int execOrder[], int execCount);

int fcfsSchedule(struct Process p[], int n, struct ExecSegment gantt[]);
int roundRobinSchedule(struct Process p[], int n, int tq, struct ExecSegment gantt[],
                       int execOrder[], int *execCount);

int main() {
    int n, algo, tq = 0;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    struct Process p[n];
    inputProcesses(p, n);

    bubbleSortByArrival(p, n);

    struct ExecSegment gantt[1000];
    int execOrder[1000], execCount = 0;
    int segCount = 0;

    printf("Select scheduling algorithm:\n1. FCFS\n2. Round Robin\n3. Exit\nChoice: ");
    scanf("%d", &algo);

    switch (algo) {
        case 1:
            segCount = fcfsSchedule(p, n, gantt);
            for (int i=0; i<segCount; i++) execOrder[i] = gantt[i].pid;
            execCount = segCount;
            break;
        case 2:
            printf("Enter Time Quantum: ");
            scanf("%d", &tq);
            segCount = roundRobinSchedule(p, n, tq, gantt, execOrder, &execCount);
            break;
        case 3:
            printf("Exiting...\n");
            return 0;
        default:
            printf("Invalid choice\n");
            return 1;
    }

    printGanttChart(gantt, segCount);
    printReadyQueueInOneLine(execOrder, execCount);
    printProcessTable(p, n);

    return 0;
}

void inputProcesses(struct Process p[], int n) {
    for (int i = 0; i < n; i++) {
        printf("Enter PID, Arrival Time, Burst Time for process %d: ", i + 1);
        scanf("%d %d %d", &p[i].pid, &p[i].at, &p[i].bt);
        p[i].remaining = p[i].bt;
        p[i].ct = p[i].tat = p[i].wt = 0;
    }
}

void bubbleSortByArrival(struct Process p[], int n) {
    for (int i = 0; i < n -1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].at > p[j+1].at) {
                struct Process temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }
}

int fcfsSchedule(struct Process p[], int n, struct ExecSegment gantt[]) {
    int time = 0, segCount = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].at) time = p[i].at;
        gantt[segCount].pid = p[i].pid;
        gantt[segCount].start = time;
        time += p[i].bt;
        gantt[segCount].end = time;
        p[i].ct = time;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        segCount++;
    }
    return segCount;
}

int roundRobinSchedule(struct Process p[], int n, int tq, struct ExecSegment gantt[],
                       int execOrder[], int *execCount) {
    int time = p[0].at, completed = 0, segCount = 0;
    int queue[100], front = 0, rear = 0, visited[100] = {0};
    *execCount = 0;

    queue[rear++] = 0;
    visited[0] = 1;

    while (completed < n) {
        if (front == rear) {
            int nextArrival = -1;
            for (int i = 0; i < n; i++) {
                if (!visited[i]) {
                    if (nextArrival == -1 || p[i].at < nextArrival)
                        nextArrival = p[i].at;
                }
            }
            if (nextArrival == -1) break;
            time = nextArrival;
            for (int i=0; i<n; i++) {
                if (!visited[i] && p[i].at <= time) {
                    queue[rear++] = i;
                    visited[i] = 1;
                }
            }
            continue;
        }

        int idx = queue[front++];
        int execTime = (p[idx].remaining > tq) ? tq : p[idx].remaining;

        gantt[segCount].pid = p[idx].pid;
        gantt[segCount].start = time;
        time += execTime;
        gantt[segCount].end = time;
        segCount++;

        execOrder[(*execCount)++] = p[idx].pid;

        p[idx].remaining -= execTime;

        for (int i = 0; i < n; i++) {
            if (!visited[i] && p[i].at <= time) {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }

        if (p[idx].remaining > 0) {
            queue[rear++] = idx;
        } else {
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            completed++;
        }
    }
    return segCount;
}

void printGanttChart(struct ExecSegment gantt[], int segCount) {
    printf("\nGantt Chart:\n ");
    for (int i = 0; i < segCount; i++) printf("------");
    printf("-\n|");
    for (int i = 0; i < segCount; i++) printf(" P%-2d |", gantt[i].pid);
    printf("\n ");
    for (int i = 0; i < segCount; i++) printf("------");
    printf("-\n");
    for (int i = 0; i < segCount; i++) printf("%-6d", gantt[i].start);
    if (segCount > 0) printf("%d\n", gantt[segCount - 1].end);
}

void printReadyQueueInOneLine(int execOrder[], int execCount) {
    printf("\nReady Queue: ");
    for (int i = 0; i < execCount; i++) {
        printf("P%d", execOrder[i]);
        if (i != execCount - 1) printf(" | ");
    }
    printf("\n");
}

void printProcessTable(struct Process p[], int n) {
    float totalWT = 0, totalTAT = 0;
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
        totalWT += p[i].wt;
        totalTAT += p[i].tat;
    }
    printf("\nAverage Turnaround Time: %.2f\n", totalTAT / n);
    printf("Average Waiting Time: %.2f\n", totalWT / n);
}
