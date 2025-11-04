#include <stdio.h>

struct Process{
    int pid, at, bt, priority, ct, tat, wt, remaining;
};

struct ExecSegment{
    int pid, start, end;
};

int priorityPreemptive(struct Process p[], int n, struct ExecSegment gantt[], int execOrder[], int *execCount){
    int rem[100], time=0, completed=0, s=0, last=-1;
    *execCount = 0;

    for(int i=0;i<n;i++){
        rem[i] = p[i].bt;
        p[i].remaining = p[i].bt;
    }

    while(completed < n){
        int idx=-1, minPr=1e9;
        for(int i=0;i<n;i++){
            if(p[i].at <= time && rem[i]>0 && p[i].priority < minPr){
                minPr = p[i].priority;
                idx = i;
            }
        }

        if(idx==-1){
            time++;
            continue;
        }

        if(last != p[idx].pid){
            gantt[s].pid = p[idx].pid;
            gantt[s].start = time;
            s++;
            last = p[idx].pid;
        }

        time++;
        rem[idx]--;
        execOrder[(*execCount)++] = p[idx].pid;
        gantt[s-1].end = time;

        if(rem[idx]==0){
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            completed++;
        }
    }

    return s;
}

// Helper functions (same as NP)
void printGantt(struct ExecSegment gantt[], int segCount){
    printf("\nGantt Chart:\n ");
    for(int i=0;i<segCount;i++) printf("------"); printf("-\n|");
    for(int i=0;i<segCount;i++) printf(" P%-2d |", gantt[i].pid); printf("\n ");
    for(int i=0;i<segCount;i++) printf("------"); printf("-\n");
    for(int i=0;i<segCount;i++) printf("%-6d", gantt[i].start);
    if(segCount>0) printf("%d\n", gantt[segCount-1].end);
}

void printReadyQueue(int execOrder[], int execCount){
    if(execCount == 0) return;
    printf("\nReady Queue: P%d", execOrder[0]);
    for(int i = 1; i < execCount; i++){
        if(execOrder[i] != execOrder[i-1])  // only print if different from previous
            printf(" | P%d", execOrder[i]);
    }
    printf("\n");
}


void printTable(struct Process p[], int n){
    float totalTAT=0,totalWT=0;
    printf("\nPID\tAT\tBT\tPR\tCT\tTAT\tWT\n");
    for(int i=0;i<n;i++){
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].priority, p[i].ct, p[i].tat, p[i].wt);
        totalTAT += p[i].tat;
        totalWT += p[i].wt;
    }
    printf("\nAverage TAT: %.2f\nAverage WT: %.2f\n", totalTAT/n, totalWT/n);
}

int main(){
    int n;
    printf("Enter number of processes: ");
    scanf("%d",&n);
    struct Process p[n];
    for(int i=0;i<n;i++){
        printf("Enter PID, Arrival Time, Burst Time, Priority for process %d: ", i+1);
        scanf("%d %d %d %d",&p[i].pid,&p[i].at,&p[i].bt,&p[i].priority);
    }

    struct ExecSegment gantt[1000];
    int execOrder[1000], execCount=0;
    int segCount = priorityPreemptive(p,n,gantt,execOrder,&execCount);

    printGantt(gantt,segCount);
    printReadyQueue(execOrder,execCount);
    printTable(p,n);

    return 0;
}
