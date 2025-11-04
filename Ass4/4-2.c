#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define CHAIRS 3
#define STUDENTS 5
sem_t ta;
sem_t students;
pthread_mutex_t mutex;

int waiting = 0; //number of students waiting

void *ta_work(void *arg){
    while(1){
        sem_wait(&students);
        pthread_mutex_lock(&mutex);
        
        waiting--; //one student goies in for help
        printf("TA is helping a student. Waiting students: %d\n", waiting);
        
        pthread_mutex_unlock(&mutex);
        sem_post(&ta);               // Signal student that TA is ready
        
        sleep(2);
        printf("Finished helping waiting for next student.\n");
    }
    return NULL;
}

void *student_work(void *arg){
    int student_id = *(int *)arg;
    
    while(1){
        pthread_mutex_lock(&mutex);
        if(waiting < CHAIRS){
            waiting++;
            printf("Student %d waiting. Chairs left: %d\n", student_id, CHAIRS - waiting);
            pthread_mutex_unlock(&mutex);   
            
            sem_post(&students);
            sem_wait(&ta); //wait for ta to finish
            printf("Student %d got help and leaves.\n", student_id);
        }else{
            pthread_mutex_unlock(&mutex);
            printf("No chair avalaible. Student %d will come back later.\n", student_id);
        }
        // Student does some work before coming back
        sleep(rand() % 5 + 1);
    }
    return NULL;
}

int main(){
    pthread_t ta_thread;
    pthread_t students_threads[STUDENTS];
    int student_ids[STUDENTS];
    
    sem_init(&students, 0, 0);
    sem_init(&ta, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Create TA thread
    pthread_create(&ta_thread, NULL, ta_work, NULL);

    // Create Student threads
    for (int i = 0; i < STUDENTS; i++) {
        student_ids[i] = i + 1;
        pthread_create(&students_threads[i], NULL, student_work, &student_ids[i]);
    }
    
    pthread_join(ta_thread, NULL);
    for (int i = 0; i < STUDENTS; i++)
        pthread_join(students_threads[i], NULL);
    return 0;
}