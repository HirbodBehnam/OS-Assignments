#include <stdlib.h>
#include <pthread.h>
#include "linked_list.h"
#include "util.h"

LinkedList_t *list;

void make_list();

int main(int argc, char **argv) {
    // Parse arguments
    struct ParsedArguments arguments = parse_arguments(argc, argv);
    // Create the list
    make_list();
    // Spawn threads

    // Wait for them
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
}