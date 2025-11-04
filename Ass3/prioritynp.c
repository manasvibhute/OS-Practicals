#include <stdio.h>

struct Process{
    int pid, at, bt, priority, ct, tat, wt;
};

struct ExecSegment{
    int pid, start, end;
};

int priorityNonPreemptive(struct Process p[], int n, struct ExecSegment gantt[], int execOrder[], int *execCount){
    int done[100]={0}, time=0, s=0, completed=0;
    *execCount = 0;

    while(completed < n){
        int idx=-1, minPr = 1e9;
        for(int i=0;i<n;i++){
            if(!done[i] && p[i].at <= time && p[i].priority < minPr){
                minPr = p[i].priority;
                idx = i;
            }
        }

        if(idx==-1){
            time++;
            continue;
        }

        gantt[s].pid = p[idx].pid;
        gantt[s].start = time;
        time += p[idx].bt;
        gantt[s].end = time;
        s++;

        for(int i=0;i<p[idx].bt;i++) execOrder[(*execCount)++] = p[idx].pid;

        p[idx].ct = time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        done[idx]=1;
        completed++;
    }

    return s;
}

// Helper functions
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
    int segCount = priorityNonPreemptive(p,n,gantt,execOrder,&execCount);

    printGantt(gantt,segCount);
    printReadyQueue(execOrder,execCount);
    printTable(p,n);

    return 0;
}
