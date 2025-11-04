#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg.h"

void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = toupper((unsigned char)str[i]); //(unsigned char) cast avoids problems with non-ASCII inputs.
}

int main() {
    int msgid;
    struct msgbuf message;

    // Open the same message queue (created by sender)
    msgid = msgget(KEY, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    while (1) {
        // Receive message of type 1
        if (msgrcv(msgid, &message, MSG_SIZE, 1, 0) == -1) {
        //ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
            perror("msgrcv");
            exit(1);
        }

        if (strcmp(message.mtext, "exit") == 0)
            break;

        // Convert to uppercase
        to_uppercase(message.mtext);
        printf("Received (uppercase): %s\n", message.mtext);
    }

    // Delete message queue (cleanup)
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
    //int msgctl(int msqid, int cmd, struct msqid_ds *buf);
        perror("msgctl");
        exit(1);
    }

    printf("Receiver finished. Queue deleted.\n");
    return 0;
}
