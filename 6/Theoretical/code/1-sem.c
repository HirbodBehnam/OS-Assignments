#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TOTAL_DRINKS 4
#define CUSTOMERS 3
sem_t drink_queue_sem, bartender_caller_sem, bartender_done_sem;
int drinks = TOTAL_DRINKS;

void drink() {
    sleep(5);
}

void *customer(void *args) {
    int id = *(int *) args;
    while (1) {
        // Get drink loop
        printf("%d wants to drink\n", id);
        sem_wait(&drink_queue_sem);
        while (drinks == 0) {
            printf("%d wants to signal the bartender\n", id);
            sem_post(&bartender_caller_sem); // Call the bartender
            printf("%d signaled the bartender\n", id);
            sem_wait(&bartender_done_sem); // Wait for bartender to fill everything
            printf("%d wake up\n", id);
        }
        printf("%d got a drink!\n", id);
        drinks--;
        sem_post(&drink_queue_sem);
        // Drink
        drink();
    }
}

void *bartender(void *args) {
    while (1) {
        // Wait for a customer to call bartender
        sem_wait(&bartender_caller_sem);
        printf("Bartender refilled drinks\n");
        drinks = 5;
        // Tell the customer that we have filled everything
        sem_post(&bartender_done_sem);
    }
}

int main() {
    sem_init(&drink_queue_sem, 0, 1);
    sem_init(&bartender_caller_sem, 0, 0);
    sem_init(&bartender_done_sem, 0, 0);
    pthread_t thread_id;
    for (int i = 0; i < CUSTOMERS; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&thread_id, NULL, customer, id);
    }
    pthread_create(&thread_id, NULL, bartender, NULL);
    sleep(200000000);
}