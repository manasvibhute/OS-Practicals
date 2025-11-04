#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

int readcount = 0;
int shared_data = 0;

sem_t x,wsem;

void *reader(void *arg){
    int id = *(int *)arg;
    while(1){
    sem_wait(&x);
    readcount++;
    if(readcount == 1){
        sem_wait(&wsem);
    }
    sem_post(&x);
    
    
    printf("Reader read %d at index %d\n", id, shared_data);
    sleep(1);
    
    sem_wait(&x);
    readcount--;
    if(readcount == 0){
        sem_post(&wsem);
    }
    sem_post(&x);
    sleep(1);
    }
    return NULL;
}

void *writer(void *arg){
    int id = *(int *)arg;
    while(1){
        sem_wait(&wsem);
        
        shared_data++;
        printf("Writer %d is writing shared_data = %d\n", id, shared_data);
        sleep(1);
        sem_post(&wsem);
        sleep(2);
    }return NULL;
}

int main(){
    int nr = 5;
    int nw = 2;
    pthread_t *r = malloc(nr * sizeof(pthread_t));
    pthread_t *w = malloc(nw * sizeof(pthread_t));
    int r_ids[5] = {1,2,3,4,5};
    int w_ids[2] = {1,2};
    
    sem_init(&x, 0, 1);
    sem_init(&wsem, 0, 1);
    
    for(int i=0; i<nr; i++){
        pthread_create(&r[i], NULL, reader, &r_ids[i]);
    }
    for(int i=0; i<nw; i++){
        pthread_create(&w[i], NULL, writer, &w_ids[i]);
    }
    
    for (int i = 0; i < 5; i++)
        pthread_join(r[i], NULL);
    for (int i = 0; i < 2; i++)
        pthread_join(w[i], NULL);
        
    return 0;
}