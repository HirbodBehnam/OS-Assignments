#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TOTAL_DRINKS 4
#define CUSTOMERS 3
pthread_mutex_t lock;
pthread_cond_t cond;
int drinks = TOTAL_DRINKS;

void drink() {
    sleep(5);
}

void *customer(void *args) {
    int id = *(int *) args;
    while (1) {
        // Get drink loop
        printf("%d wants to drink\n", id);
        pthread_mutex_lock(&lock);
        while (drinks == 0) {
            printf("%d wants to signal the bartender\n", id);
            pthread_cond_signal(&cond);
            printf("%d signaled the bartender\n", id);
            pthread_cond_wait(&cond, &lock);
            printf("%d wake up\n", id);
        }
        printf("%d got a drink!\n", id);
        drinks--;
        pthread_mutex_unlock(&lock);
        // Drink
        drink();
    }
}

void *bartender(void *args) {
    while (1) {
        pthread_mutex_lock(&lock);
        while (drinks != 0)
            pthread_cond_wait(&cond, &lock);
        drinks = TOTAL_DRINKS;
        printf("bartender refilled drinks\n");
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);
    }
}

int main() {
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_t thread_id;
    for (int i = 0; i < CUSTOMERS; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&thread_id, NULL, customer, id);
    }
    pthread_create(&thread_id, NULL, bartender, NULL);
    sleep(200000000);
}