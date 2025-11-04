#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

sem_t data_ready;     // Binary semaphore - signals that data is available
sem_t data_consumed;  // Binary semaphore - signals that data has been read
pthread_mutex_t mutex;

FILE *file;

void *producer(void *arg) {
    int num;
    while (1) {
        num = rand() % 10;  // generate random number 0–9

        sleep(1);  // 1 second delay between numbers

        // Wait until consumer has consumed the previous number
        sem_wait(&data_consumed);

        pthread_mutex_lock(&mutex);
        file = fopen("data.txt", "w");
        if (file == NULL) {
            perror("Error opening file for writing");
            exit(1);
        }
        fprintf(file, "%d\n", num);
        fclose(file);
        printf("Producer: produced %d\n", num);
        pthread_mutex_unlock(&mutex);

        // Signal that new data is ready
        sem_post(&data_ready);
    }
    return NULL;
}

void *consumer(void *arg) {
    int num;
    while (1) {
        // Wait until data is ready
        sem_wait(&data_ready);

        pthread_mutex_lock(&mutex);
        file = fopen("data.txt", "r");
        if (file == NULL) {
            perror("Error opening file for reading");
            exit(1);
        }
        fscanf(file, "%d", &num);
        fclose(file);
        printf("Consumer: consumed %d\n", num);
        pthread_mutex_unlock(&mutex);

        // Signal that data is consumed
        sem_post(&data_consumed);

        // Small delay so output is readable
        sleep(2);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t prod, cons;

    // Binary semaphores
    sem_init(&data_ready, 0, 0);
    sem_init(&data_consumed, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}
