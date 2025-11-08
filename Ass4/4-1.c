#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE 5

sem_t empty, full;
pthread_mutex_t mutex;
int in = 0, out = 0;

int counter[BUFFER_SIZE]; // Shared pizza counter 🍕

void print_counter() {
    printf("\n Pizza Counter: [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (counter[i] == 0)
            printf("_");  // Empty slot
        else
            printf("🍕%d", counter[i]); // Pizza number
        if (i != BUFFER_SIZE - 1) printf(" | ");
    }
    printf("]\n");
}

void *producer(void *arg) {
    int pizza = 0;
    while (1) {
        pizza++;
        sem_wait(&empty);              // Wait for an empty slot
        pthread_mutex_lock(&mutex);    // Enter critical section

        counter[in] = pizza;
        printf("\n Mr. Simpson baked Pizza #%d and placed it at position %d\n", pizza, in);
        print_counter();

        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);  // Exit critical section
        sem_post(&full);               // Signal that one pizza is ready

        sleep(1); // Time to bake next pizza
    }
    return NULL;
}

void *consumer(void *arg) {
    int pizza;
    while (1) {
        sem_wait(&full);               // Wait for pizza to be available
        pthread_mutex_lock(&mutex);    // Enter critical section

        pizza = counter[out];
        counter[out] = 0; // Mark slot empty after Joey eats
        printf("\n Joey ate Pizza #%d from position %d\n", pizza, out);
        print_counter();

        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);  // Exit critical section
        sem_post(&empty);              // Signal that one slot is free

        sleep(2); // Time taken to eat
    }
    return NULL;
}

int main() {
    pthread_t simpson, joey;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    printf("\n === Mr. Simpson and Joey Tribbiani: The Pizza Counter Simulation === \n");
    printf("Counter size: %d pizzas\n\n", BUFFER_SIZE);

    pthread_create(&simpson, NULL, producer, NULL);
    pthread_create(&joey, NULL, consumer, NULL);

    pthread_join(simpson, NULL);
    pthread_join(joey, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
