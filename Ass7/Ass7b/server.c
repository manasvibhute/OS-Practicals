#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define KEY 1234
#define SIZE 1024

int main() {
    int shmid;
    char *shm_ptr;

    // Create shared memory
    shmid = shmget(KEY, SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    // Attach shared memory
    shm_ptr = (char *) shmat(shmid, NULL, 0);
    if (shm_ptr == (char *) -1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    printf("Server: Writing message to shared memory...\n");
    sleep(1); // Simulate delay

    // Write message
    char message[] = "Hello Client! Message from Server. How are you?";
    strcpy(shm_ptr + 1, message); // Leave first byte for sync flag

    shm_ptr[0] = '1'; // Flag to indicate message ready
    printf("Server: Message written, waiting for client to read...\n");

    // Wait until client sets flag to '2'
    while (shm_ptr[0] != '2') {
        sleep(1);
    }

    printf("Server: Client has read the message.\n");

    // Detach and remove
    shmdt(shm_ptr);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

