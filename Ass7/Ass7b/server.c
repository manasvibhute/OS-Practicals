 #include <stdio.h>
 #include <stdlib.h>
 #include <sys/ipc.h>
 #include <sys/shm.h>
 #include <string.h>
 
#define SHM_KEY 1234       // Key for shared memory
 #define SHM_SIZE 1024      // Size of shared memory
 
int main() {
    int shmid;
    char *shm_ptr;
 
    // Create shared memory segment
    shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }
 
    // Attach shared memory to server's address space
    shm_ptr = (char *) shmat(shmid, NULL, 0);
    if (shm_ptr == (char *) -1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }
 
    // Write message to shared memory
    char message[] = "Hello Client! Message from Server.How are you?";
    strncpy(shm_ptr, message, SHM_SIZE);
 
    printf("Server: Message written to shared memory.\n");
    printf("Press Enter to detach and exit...\n");
    getchar();
 
    // Detach shared memory
    shmdt(shm_ptr);
 
    // Optional: Remove shared memory segment
    // shmctl(shmid, IPC_RMID, NULL);
 
    return 0;
}