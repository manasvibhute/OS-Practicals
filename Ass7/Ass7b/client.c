#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 1234
#define SHM_SIZE 1024

int main() {
    int shmid;
    char *shm_ptr;

    // Get the shared memory segment created by server
    shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid < 0) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    // Attach shared memory to client's address space
    shm_ptr = (char *) shmat(shmid, NULL, 0);
    if (shm_ptr == (char *) -1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    // Read message from shared memory
    printf("Client: Message from Server: \"%s\"\n", shm_ptr);

    // Detach shared memory
    shmdt(shm_ptr);

    return 0;
}
