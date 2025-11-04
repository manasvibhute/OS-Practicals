#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>

#define buffer 5
sem_t empty, full;
pthread_mutex_t mutex;
int in=0, out=0;

int arr[buffer];

// Helper function to print the buffer state
void print_buffer() {
    printf("Buffer: [");
    for (int i = 0; i < buffer; i++) {
        if (arr[i] == 0)
            printf("0");
        else
            printf("%d", arr[i]);
        if (i != buffer - 1) printf(", ");
    }
    printf("]\n");
}

void *producer(void *arg){
    int item = 0;
    do{
        item++;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        arr[in] = item;
        printf("Producer produced %d at index %d\n", item, in);
        print_buffer();

        in = (in+1) % buffer;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(1);
    }while(1);
    return NULL;
}

void *consumer(void *arg){
    int item;
    do{
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        item = arr[out];
        arr[out] = 0; // Mark slot as empty after consuming
        printf("Consumer consumed %d from index %d\n", item, out);
        print_buffer();

        out = (out+1) % buffer;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(2);
    }while(1);
    return NULL;
}

int main(){
    pthread_t prod, cons;
    sem_init(&empty, 0, buffer);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}