#include <stdio.h>
#include <stdlib.h>
#define MAX 100

int absdiff(int a, int b) { 
    return (a > b) ? a - b : b - a; 
}

// ------------------- SCAN -------------------
void SCAN(int req[], int n, int head, int disk, char dir){
    int left[MAX], right[MAX], lcnt = 0, rcnt = 0, total = 0;
    for (int i = 0; i < n; i++){
        if (req[i] < head)
            left[lcnt++] = req[i];
        else
            right[rcnt++] = req[i];
    }

    // sort both sides
    for (int i = 0; i < lcnt - 1; i++)
        for (int j = 0; j < lcnt - i - 1; j++)
            if (left[j] < left[j + 1]){
                int t = left[j];
                left[j] = left[j + 1];
                left[j + 1] = t;
            }

    for (int i = 0; i < rcnt - 1; i++)
        for (int j = 0; j < rcnt - i - 1; j++)
            if (right[j] > right[j + 1]){
                int t = right[j];
                right[j] = right[j + 1];
                right[j + 1] = t;
            }

    printf("\nSCAN sequence: %d", head);
    if (dir == 'R' || dir == 'r'){
        for (int i = 0; i < rcnt; i++){
            total += absdiff(right[i], head);
            head = right[i];
            printf(" -> %d", head);
        }
        if (head != disk - 1){
            total += absdiff(disk - 1, head);
            head = disk - 1;
            printf(" -> %d", head);
        }
        for (int i = 0; i < lcnt; i++){
            total += absdiff(left[i], head);
            head = left[i];
            printf(" -> %d", head);
        }
    }
    else{
        for (int i = 0; i < lcnt; i++){
            total += absdiff(left[i], head);
            head = left[i];
            printf(" -> %d", head);
        }
        if (head != 0){
            total += absdiff(0, head);
            head = 0;
            printf(" -> 0");
        }
        for (int i = 0; i < rcnt; i++){
            total += absdiff(right[i], head);
            head = right[i];
            printf(" -> %d", head);
        }
    }
    printf("\nTotal Head Movement (SCAN): %d\n", total);
}

// ------------------- LOOK -------------------
void LOOK(int req[], int n, int head, char dir){
    int left[MAX], right[MAX], lcnt = 0, rcnt = 0, total = 0;
    for (int i = 0; i < n; i++){
        if (req[i] < head)
            left[lcnt++] = req[i];
        else
            right[rcnt++] = req[i];
    }

    for (int i = 0; i < lcnt - 1; i++)
        for (int j = 0; j < lcnt - i - 1; j++)
            if (left[j] < left[j + 1]){
                int t = left[j];
                left[j] = left[j + 1];
                left[j + 1] = t;
            }

    for (int i = 0; i < rcnt - 1; i++)
        for (int j = 0; j < rcnt - i - 1; j++)
            if (right[j] > right[j + 1]){
                int t = right[j];
                right[j] = right[j + 1];
                right[j + 1] = t;
            }

    printf("\nLOOK sequence: %d", head);
    if (dir == 'R' || dir == 'r'){
        for (int i = 0; i < rcnt; i++){
            total += absdiff(right[i], head);
            head = right[i];
            printf(" -> %d", head);
        }
        for (int i = 0; i < lcnt; i++){
            total += absdiff(left[i], head);
            head = left[i];
            printf(" -> %d", head);
        }
    }
    else{
        for (int i = 0; i < lcnt; i++){
            total += absdiff(left[i], head);
            head = left[i];
            printf(" -> %d", head);
        }
        for (int i = 0; i < rcnt; i++){
            total += absdiff(right[i], head);
            head = right[i];
            printf(" -> %d", head);
        }
    }
    printf("\nTotal Head Movement (LOOK): %d\n", total);
}

// ------------------- C-SCAN -------------------
void CSCAN(int req[], int n, int head, int disk){
    int left[MAX], right[MAX], lcnt = 0, rcnt = 0, total = 0;
    for (int i = 0; i < n; i++){
        if (req[i] < head)
            left[lcnt++] = req[i];
        else
            right[rcnt++] = req[i];
    }

    for (int i = 0; i < lcnt - 1; i++)
        for (int j = 0; j < lcnt - i - 1; j++)
            if (left[j] > left[j + 1]){
                int t = left[j];
                left[j] = left[j + 1];
                left[j + 1] = t;
            }

    for (int i = 0; i < rcnt - 1; i++)
        for (int j = 0; j < rcnt - i - 1; j++)
            if (right[j] > right[j + 1]){
                int t = right[j];
                right[j] = right[j + 1];
                right[j + 1] = t;
            }

    printf("\nC-SCAN sequence: %d", head);
    for (int i = 0; i < rcnt; i++){
        total += absdiff(right[i], head);
        head = right[i];
        printf(" -> %d", head);
    }
    if (head != disk - 1){
        total += absdiff(disk - 1, head);
        head = disk - 1;
        printf(" -> %d", head);
    }

    head = 0;
    printf(" -> 0");
    
    for (int i = 0; i < lcnt; i++){
        total += absdiff(left[i], head);
        head = left[i];
        printf(" -> %d", head);
    }

    printf("\nTotal Head Movement (C-SCAN): %d\n", total);
}

// ------------------- C-LOOK -------------------
void CLOOK(int req[], int n, int head){
    int left[MAX], right[MAX], lcnt = 0, rcnt = 0, total = 0;
    for (int i = 0; i < n; i++){
        if (req[i] < head)
            left[lcnt++] = req[i];
        else
            right[rcnt++] = req[i];
    }

    for (int i = 0; i < lcnt - 1; i++)
        for (int j = 0; j < lcnt - i - 1; j++)
            if (left[j] > left[j + 1]){
                int t = left[j];
                left[j] = left[j + 1];
                left[j + 1] = t;
            }

    for (int i = 0; i < rcnt - 1; i++)
        for (int j = 0; j < rcnt - i - 1; j++)
            if (right[j] > right[j + 1]){
                int t = right[j];
                right[j] = right[j + 1];
                right[j + 1] = t;
            }

    printf("\nC-LOOK sequence: %d", head);
    for (int i = 0; i < rcnt; i++){
        total += absdiff(right[i], head);
        head = right[i];
        printf(" -> %d", head);
    }
    if (lcnt > 0){
        head = left[0];
        printf(" -> %d", head);
    }
    for (int i = 1; i < lcnt; i++){
        total += absdiff(left[i], head);
        head = left[i];
        printf(" -> %d", head);
    }

    printf("\nTotal Head Movement (C-LOOK): %d\n", total);
}

// ------------------- MAIN -------------------
int main(){
    int n, head, disk;
    int req[MAX];
    char choice, dir;

    printf("Enter number of requests: ");
    scanf("%d", &n);
    printf("Enter request sequence: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &req[i]);
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("Enter total disk size: ");
    scanf("%d", &disk);

    printf("\nChoose Algorithm:\n");
    printf("1. SCAN\n2. LOOK\n3. C-SCAN\n4. C-LOOK\nEnter choice: ");
    scanf(" %c", &choice);

    switch (choice)
    {
    case '1':
        printf("Enter direction (L/R): ");
        scanf(" %c", &dir);
        SCAN(req, n, head, disk, dir);
        break;
    case '2':
        printf("Enter direction (L/R): ");
        scanf(" %c", &dir);
        LOOK(req, n, head, dir);
        break;
    case '3':
        CSCAN(req, n, head, disk);
        break;
    case '4':
        CLOOK(req, n, head);
        break;
    default:
        printf("Invalid choice!\n");
    }

    return 0;
}
