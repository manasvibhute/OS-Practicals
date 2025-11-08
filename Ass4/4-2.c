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

void *student_work(void *arg){
    int student_id = *(int *)arg;
    
    while(1){
        pthread_mutex_lock(&mutex); //lock so One student checks if there’s space.
        if(waiting < CHAIRS){
            waiting++;
            printf("[Student %d] waiting. Chairs left: %d\n", student_id, CHAIRS - waiting);
            pthread_mutex_unlock(&mutex);   //unlock so others can check
            
            sem_post(&students);/*Increase the value of the students semaphore by 1 Or Tell the TA that one more student is waiting.)*/
            sem_wait(&ta); // student waits for TA’s response
            printf("[Student %d] got help and leaves.\n", student_id);
        }else{
            pthread_mutex_unlock(&mutex); //The student unlocks the door (so others can check),
            printf("[Student %d] No chair avalaible.Will come back later.\n", student_id);
        }
        // Student does some work before coming back
        sleep(rand() % 5 + 1);
    }
    return NULL;
}

void *ta_work(void *arg){
    while(1){
        sem_wait(&students); //Wait until at least one student arrives.
        pthread_mutex_lock(&mutex); //“I’ll lock the waiting room list — I’m going to take one student in.”
        
        waiting--; //one student goes in for help
        printf("TA is helping a student. Waiting students: %d\n", waiting);
        
        pthread_mutex_unlock(&mutex); //“I’m done changing the waiting count — students can now check chairs again.
        sem_post(&ta);               // Signal student that TA is ready
        
        sleep(2);
        printf("Finished helping waiting for next student.\n");
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

/*(Make the output like this)
[TA] No students waiting. TA is sleeping...
[Student 1] Arrived and found a seat. Chairs left: 2
[TA] A student enters the office. Students waiting outside: 0
[Student 1] Got help from TA and leaves the office.
[Student 2] Arrived and found a seat. Chairs left: 2
[TA] Helping a student with their assignment...
[Student 3] Arrived and found a seat. Chairs left: 1
[Student 4] Arrived and found a seat. Chairs left: 0
[Student 5] No chairs available. Will come back later.
[TA] Finished helping the student. TA is ready for the next one.
[TA] A student enters the office. Students waiting outside: 2
[TA] Helping a student with their assignment...
[Student 2] Got help from TA and leaves the office.
[Student 1] Arrived and found a seat. Chairs left: 0
[Student 5] No chairs available. Will come back later.
[TA] Finished helping the student. TA is ready for the next one.
[TA] A student enters the office. Students waiting outside: 2
[TA] Helping a student with their assignment...
[Student 3] Got help from TA and leaves the office.*/