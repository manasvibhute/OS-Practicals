#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define KEY 1234
#define SIZE 1024

int main() {
    int shmid;
    char *shm_ptr;

    // Access shared memory
    shmid = shmget(KEY, SIZE, 0666);
    if (shmid < 0) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    // Attach
    shm_ptr = (char *) shmat(shmid, NULL, 0);
    if (shm_ptr == (char *) -1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    printf("Client: Waiting for server to write message...\n");

    /*| Flag  | Meaning                    |
    | ----- | -------------------------- |
    | `'0'` | Nothing yet / waiting      |
    | `'1'` | Server has written message |
    | `'2'` | Client has read message    |
    */
    // Wait until server sets flag = '1'
    while (shm_ptr[0] != '1') {
        sleep(1);
    }

    printf("Client: Message from Server: \"%s\"\n", shm_ptr + 1);

    // Set flag = '2' to tell server we're done
    shm_ptr[0] = '2';

    // Detach
    shmdt(shm_ptr);

    return 0;
}