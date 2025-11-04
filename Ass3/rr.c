#include <stdio.h>

struct Process{
    int pid, at, bt, remaining, ct, tat, wt;
};

struct ExecSegment{
    int pid, start, end;
};

int roundRobin(struct Process p[], int n, int tq, struct ExecSegment gantt[], int execOrder[], int *execCount){
    int queue[100], front=0, rear=0, visited[100]={0};
    int time = 0, completed=0, s=0;
    *execCount = 0;

    // Initially push processes that arrived at time 0
    for(int i=0;i<n;i++){
        p[i].remaining = p[i].bt;
        if(p[i].at <= time){
            queue[rear++] = i;
            visited[i] = 1;
        }
    }

    while(completed < n){
        if(front == rear){
            // No process in queue; move time to next arrival
            int nextArrival = 1e9;
            for(int i=0;i<n;i++){
                if(!visited[i] && p[i].at < nextArrival) nextArrival = p[i].at;
            }
            time = nextArrival;
            for(int i=0;i<n;i++){
                if(!visited[i] && p[i].at <= time){
                    queue[rear++] = i;
                    visited[i] = 1;
                }
            }
            continue;
        }

        int idx = queue[front++]; //dequed
        int exec = (p[idx].remaining > tq) ? tq : p[idx].remaining;

        gantt[s].pid = p[idx].pid;
        gantt[s].start = time;
        time += exec;
        gantt[s].end = time;
        s++;

        for(int i=0;i<exec;i++) execOrder[(*execCount)++] = p[idx].pid;

        p[idx].remaining -= exec;

        // Check for newly arrived processes
        for(int i=0;i<n;i++){
            if(!visited[i] && p[i].at <= time){
                queue[rear++] = i;
                visited[i] = 1;
            }
        }

        if(p[idx].remaining > 0) queue[rear++] = idx;
        else{
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            completed++;
        }
    }

    return s;
}

// Helper functions for printing (Gantt, Ready Queue, Table)
void printGantt(struct ExecSegment gantt[], int segCount){
    printf("\nGantt Chart:\n ");
    for(int i=0;i<segCount;i++) printf("------"); printf("-\n|");
    for(int i=0;i<segCount;i++) printf(" P%-2d |", gantt[i].pid); printf("\n ");
    for(int i=0;i<segCount;i++) printf("------"); printf("-\n");
    for(int i=0;i<segCount;i++) printf("%-6d", gantt[i].start);
    if(segCount>0) printf("%d\n", gantt[segCount-1].end);
}

void printReadyQueue(int execOrder[], int execCount){
    printf("\nReady Queue (execution order): ");
    for(int i=0;i<execCount;i++){
        printf("P%d%s", execOrder[i], (i!=execCount-1)?" | ":"");
    }
    printf("\n");
}

void printTable(struct Process p[], int n){
    float totalTAT=0, totalWT=0;
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for(int i=0;i<n;i++){
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
        totalTAT += p[i].tat;
        totalWT += p[i].wt;
    }
    printf("\nAverage TAT: %.2f\nAverage WT: %.2f\n", totalTAT/n, totalWT/n);
}

int main(){
    int n, tq;
    printf("Enter number of processes: ");
    scanf("%d",&n);
    struct Process p[n];
    for(int i=0;i<n;i++){
        printf("Enter PID, Arrival Time, Burst Time for process %d: ", i+1);
        scanf("%d %d %d",&p[i].pid,&p[i].at,&p[i].bt);
    }
    printf("Enter Time Quantum: ");
    scanf("%d",&tq);

    struct ExecSegment gantt[1000];
    int execOrder[1000], execCount=0;
    int segCount = roundRobin(p,n,tq,gantt,execOrder,&execCount);

    printGantt(gantt,segCount);
    printReadyQueue(execOrder,execCount);
    printTable(p,n);

    return 0;
}
