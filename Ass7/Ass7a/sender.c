#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>

#define KEY 1234
#define MSG_SIZE 1024

struct msgbuf {
    long mtype;
    char mtext[MSG_SIZE];
};

int main() {
    int msgid; //Will store the message queue identifier
    struct msgbuf message;

    // Create a message queue (or get it if it already exists)
    msgid = msgget(KEY, IPC_CREAT | 0666);
    /*KEY → A unique number identifying the queue (from "msg.h").
    IPC_CREAT → If the queue doesn’t exist, create it.
    0666 → File permission bits (read & write for everyone).*/

    /*msgget() returns:
    A positive integer ID if successful (used in later calls),
    -1 if error (e.g., permission denied).*/

    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Enter messages (type 'exit' to quit):\n");

    while (1) {
        printf("> ");
        fflush(stdout); //fflush(stdout) → Ensures the > prompt immediately shows up on screen, even before you press Enter (without buffering).

        // Take input from user
        fgets(message.mtext, MSG_SIZE, stdin);
        message.mtext[strcspn(message.mtext, "\n")] = '\0'; // remove newline
        /*message.mtext[strcspn(message.mtext, "\n")] = '\0';
        strcspn() finds the index of '\n' (newline character),
        Replaces it with '\0' (string terminator) → removes trailing newline.*/

        message.mtype = 1; // All messages are of type 1

        // Send the message to the queue
        if (msgsnd(msgid, &message, strlen(message.mtext) + 1, 0) == -1) {
        //syntax: int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
            perror("msgsnd");
            exit(1);
        }

        if (strcmp(message.mtext, "exit") == 0)
            break; // Stop if user typed exit
    }

    printf("Sender finished.\n");
    return 0;
}
