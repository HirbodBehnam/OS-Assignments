#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "linked_list.h"
#include "util.h"

struct ThreadArguments {
    int thread_id;
    int iteration_count;
};

LinkedList_t *list;

void make_list();

void *worker_thread(void *args) {
    // Get arguments
    struct ThreadArguments thread_arguments = *(struct ThreadArguments *) args;
    free(args);
    // Do stuff
    for (int i = 0; i < thread_arguments.iteration_count; i++) {
        char *payload = get_random_string();
        // TODO
    }
    // Done
    return NULL;
}

int main(int argc, char **argv) {
    // Parse arguments
    struct ParsedArguments arguments = parse_arguments(argc, argv);
    // Create the list
    make_list();
    // Spawn threads
    pthread_t threads[arguments.thread_count];
    for (int i = 0; i < arguments.thread_count; i++) {
        struct ThreadArguments *thread_arguments = malloc(sizeof(struct ThreadArguments));
        thread_arguments->thread_id = i;
        thread_arguments->iteration_count = arguments.iteration_count;
        pthread_create(&threads[i], NULL, worker_thread, thread_arguments);
    }
    // Wait for them
    for (int i = 0; i < arguments.thread_count; i++)
        pthread_join(threads[i], NULL);
    return 0;
}

void make_list() {
    // Allocate the list
    list = malloc(sizeof(LinkedList_t));
    list->value = NULL;
    list->prev = NULL;
    list->next = NULL;
    // Create elements
    for (int i = 0; i < 3; i++) {
        Element_t *elem = malloc(sizeof(Element_t));
        elem->value = get_random_string();
        insert(list, elem);
    }
    // Print list
    Element_t *elem = list->next;
    printf("Current list: ");
    while (1) {
        printf("%s", elem->value);
        elem = elem->next;
        if (elem->value == NULL) {
            break;
        } else {
            printf(" -> ");
        }
    }
    putchar('\n');
}