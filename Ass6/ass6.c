#include <stdio.h>
#include <stdlib.h>

// ✅ Common function: check if page is in frame
int isPresent(int frame[], int size, int page) {
    for (int i = 0; i < size; i++)
        if (frame[i] == page)
            return 1;
    return 0;
}

// ✅ Common function: print current frame state
void printFrame(int frame[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", frame[i]);
    printf("\n");
}

// ✅ Common function: print statistics
void printStats(int faults, int total) {
    printf("\nPage Faults : %d\n", faults);
    printf("Page Hits   : %d\n", total - faults);
    printf("Miss %%      : %.2f%%\n", (faults * 100.0) / total);
    printf("Hit %%       : %.2f%%\n", 100 - (faults * 100.0) / total);
}

// 🟩 FIFO Algorithm
void fifo(int pages[], int n, int framesize) {
    int frame[framesize], front = 0, faults = 0;
    for (int i = 0; i < framesize; i++) frame[i] = -1;

    printf("\n--- FIFO Page Replacement ---\n");
    for (int i = 0; i < n; i++) {
        if (!isPresent(frame, framesize, pages[i])) {
            frame[front] = pages[i];
            front = (front + 1) % framesize;
            faults++;
            printf("M : ");
        } else {
            printf("H : ");
        }
        printFrame(frame, framesize);
    }
    printStats(faults, n);
}

// 🟦 LRU Algorithm
int findLRU(int time[], int size) {
    int min = time[0], pos = 0;
    for (int i = 1; i < size; i++)
        if (time[i] < min) { min = time[i]; pos = i; }
    return pos;
}

void lru(int pages[], int n, int framesize) {
    int frame[framesize], time[framesize], counter = 0, faults = 0;
    for (int i = 0; i < framesize; i++) frame[i] = -1;

    printf("\n--- LRU Page Replacement ---\n");
    for (int i = 0; i < n; i++) {
        if (!isPresent(frame, framesize, pages[i])) {
            int j;
            for (j = 0; j < framesize; j++){
                if (frame[j] == -1) { //if there is an empty slot
                    frame[j] = pages[i]; //put the pages here
                    time[j] = counter++; //record current time in time[]
                    break; 
                }
            }

            if (j == framesize) { //all slots are full
                int pos = findLRU(time, framesize); //Find the page that was used least recently
                frame[pos] = pages[i]; //replace it with the new page
                time[pos] = counter++; //update its last-used time.
            }
            faults++;
            printf("M : ");
        } else { //page is already in memory
            for (int k = 0; k < framesize; k++)
                if (frame[k] == pages[i]) time[k] = counter++;
            printf("H : ");
        }
        printFrame(frame, framesize);
    }
    printStats(faults, n);
}

// 🟨 Optimal Algorithm
int findOptimal(int pages[], int n, int frame[], int index, int size) {
    int farthest = -1; // stores farthest use index
    int pos = -1;      // stores index of frame to replace

    // Loop through all pages currently in frame
    for (int i = 0; i < size; i++) {
        int j;

        // Look ahead in the reference string (from current index)
        for (j = index; j < n; j++) {
            if (frame[i] == pages[j])  // check when current frame page will appear again
                break;
        }

        // If page never appears again, replace it immediately
        if (j == n)
            return i;

        // Otherwise, choose the page that will be used farthest in the future
        if (j > farthest) {
            farthest = j;
            pos = i;
        }
    }
    return pos; // Return index of page to replace
}

void optimal(int pages[], int n, int framesize) {
    int frame[framesize], faults = 0;
    for (int i = 0; i < framesize; i++) frame[i] = -1;

    printf("\n--- Optimal Page Replacement ---\n");
    for (int i = 0; i < n; i++) {
        if (!isPresent(frame, framesize, pages[i])) {
            int j;
            // Try to fill empty slot first
            for (j = 0; j < framesize; j++) {
                if (frame[j] == -1) {
                    frame[j] = pages[i]; // put new page in empty frame
                    break;
                }
            }

            if (j == framesize) {
                int pos = findOptimal(pages, n, frame, i + 1, framesize);
                frame[pos] = pages[i];
            }
            faults++;
            printf("M : ");
        } else {
            printf("H : ");
        }
        printFrame(frame, framesize);
    }
    printStats(faults, n);
}

// 🧩 Main Function
int main() {
    int n, framesize, choice;
    printf("Enter number of pages: ");
    scanf("%d", &n);
    int pages[n];
    for (int i = 0; i < n; i++) {
        printf("Page %d: ", i + 1);
        scanf("%d", &pages[i]);
    }
    printf("Enter number of frames: ");
    scanf("%d", &framesize);

    while (1) {
        printf("\n-------------------------------\n");
        printf("Options:\n");
        printf("1. FIFO\n2. LRU\n3. Optimal\n4. Exit\nChoose: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: fifo(pages, n, framesize); break;
            case 2: lru(pages, n, framesize); break;
            case 3: optimal(pages, n, framesize); break;
            case 4: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}
