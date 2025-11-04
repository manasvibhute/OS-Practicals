#include <stdio.h>

struct Process{
    int pid, at, bt, priority, ct, tat, wt, remaining;
};

struct ExecSegment{
    int pid, start, end;
};

// ----------------- FCFS -----------------
int fcfs(struct Process p[], int n, struct ExecSegment gantt[]){
    int time = 0, s = 0;
    for(int i = 0; i < n; i++){
        if(time < p[i].at) time = p[i].at;
        gantt[s].pid = p[i].pid;
        gantt[s].start = time;
        time += p[i].bt;
        gantt[s].end = time;
        p[i].ct = time;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        s++;
    }
    return s;
}

// ----------------- SJF Non-Preemptive -----------------
int sjfNonPreemptive(struct Process p[], int n, struct ExecSegment gantt[]){
    int done[100] = {0}, time = 0, s = 0, completed = 0;
    while(completed < n){
        int idx = -1, minBT = 1e9;
        for(int i = 0; i < n; i++)
            if(!done[i] && p[i].at <= time && p[i].bt < minBT)
                minBT = p[i].bt, idx = i;
        if(idx == -1){
            time++; 
            continue; 
        }
        gantt[s].pid = p[idx].pid;
        gantt[s].start = time;
        time += p[idx].bt;
        gantt[s].end = time;
        p[idx].ct = time;
        p[idx].tat = time - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        done[idx] = 1;
        completed++;
        s++;
    }
    return s;
}

// ----------------- Round Robin -----------------
int roundRobin(struct Process p[], int n, int tq, struct ExecSegment gantt[]){
    int queue[100], front = 0, rear = 0, visited[100] = {0};
    int time = 0, s = 0, completed = 0;
    for(int i = 0; i < n; i++) p[i].remaining = p[i].bt;
    queue[rear++] = 0; visited[0] = 1; time = p[0].at;

    while(completed < n){
        if(front == rear){
            int next = -1;
            for(int i = 0; i < n; i++)
                if(!visited[i] && (next==-1 || p[i].at < next)) next = p[i].at;
            if(next == -1) break;
            time = next;    
            for(int i = 0; i < n; i++)
                if(!visited[i] && p[i].at <= time) queue[rear++] = i, visited[i] = 1;
            continue;
        }
        int idx = queue[front++];
        int exec = (p[idx].remaining > tq) ? tq : p[idx].remaining;
        gantt[s].pid = p[idx].pid;
        gantt[s].start = time;
        time += exec;
        gantt[s].end = time;
        s++;
        p[idx].remaining -= exec;
        for(int i = 0; i < n; i++)
            if(!visited[i] && p[i].at <= time) queue[rear++] = i, visited[i] = 1;
        if(p[idx].remaining > 0) queue[rear++] = idx;
        else{ p[idx].ct = time; p[idx].tat = time - p[idx].at; p[idx].wt = p[idx].tat - p[idx].bt; completed++; }
    }
    return s;
}

// ----------------- Priority Non-Preemptive -----------------
int priorityNonPreemptive(struct Process p[], int n, struct ExecSegment gantt[]){
    int priority[100], done[100] = {0}, time = 0, s = 0, completed = 0;
    for(int i = 0; i < n; i++){
        printf("Enter priority for P%d: ", p[i].pid);
        scanf("%d",&priority[i]);
    }
    while(completed < n){
        int idx = -1, minPr = 1e9;
        for(int i = 0; i < n; i++)
            if(!done[i] && p[i].at <= time && priority[i] < minPr)
                minPr = priority[i], idx = i;
        if(idx == -1){ 
            time++; 
            continue; 
        }
        gantt[s].pid = p[idx].pid;
        gantt[s].start = time;
        time += p[idx].bt;
        gantt[s].end = time;
        p[idx].ct = time;
        p[idx].tat = time - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        done[idx] = 1;
        completed++;
        s++;
    }
    return s;
}

// ----------------- Main -----------------
int main(){
    int n;
    printf("Enter number of processes: ");
    scanf("%d",&n);
    struct Process p[n];
    for(int i = 0; i < n; i++){
        printf("Enter PID, AT, BT for process %d: ", i+1);
        scanf("%d %d %d", &p[i].pid, &p[i].at, &p[i].bt);
    }

    struct ExecSegment gantt[1000];
    int segCount;

    // Example: FCFS
    segCount = sjfNonPreemptive(p,n,gantt);

    printf("\nGantt Chart:\n");
    for(int i=0;i<segCount;i++) printf("|P%d", gantt[i].pid);
    printf("|\n");

    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for(int i=0;i<n;i++)
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);

    return 0;
}
