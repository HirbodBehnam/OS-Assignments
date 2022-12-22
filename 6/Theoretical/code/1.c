sem_t drink_queue_sem = 1, bartender_caller_sem = 0, bartender_done_sem = 0;
int drinks = M;

void customer(int id) {
    while (1) {
        // Get drink
        printf("%d wants to drink\n", id);
        sem_wait(&drink_queue_sem);
        if (drinks == 0) { // If no drinks, call the bartender
            printf("%d is calling the bartender\n", id);
            sem_post(&bartender_caller_sem);
            printf("%d is waiting for bartender to fill drinks\n", id);
            sem_wait(&bartender_done_sem);
            printf("%d wake up\n", id);
        }
        printf("%d got a drink!\n", id);
        drinks--;
        sem_post(&drink_queue_sem);
        // Drink
        drink();
    }
}

void bartender() {
    while (1) {
        // Wait for a customer to call bartender
        sem_wait(&bartender_caller_sem);
        printf("Bartender refilled drinks\n");
        drinks = M;
        // Tell the customer that we have filled everything
        sem_post(&bartender_done_sem);
    }
}