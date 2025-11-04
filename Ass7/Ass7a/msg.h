// msg.h — common header for sender and receiver

#define KEY 1234
#define MSG_SIZE 100

struct msgbuf {
    long mtype;
    char mtext[MSG_SIZE];
};
