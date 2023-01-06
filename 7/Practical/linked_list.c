#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "linked_list.h"

static pthread_rwlock_t list_lock = PTHREAD_RWLOCK_INITIALIZER;

void insert(LinkedList_t *list, Element_t *element) {
    pthread_rwlock_wrlock(&list_lock);
    // Check if this list is new!
    if (list->prev == NULL) {
        // We add the element to both sides
        list->prev = element;
        list->next = element;
        element->next = list;
        element->prev = list;
        pthread_rwlock_unlock(&list_lock);
        return;
    }
    // To add new element, at first set it's prev and next value
    element->prev = list->prev;
    element->next = list; // go back to head itself
    // Add the new element as the next element of the last element
    list->prev->next = element;
    list->prev = element; // and make the list access the old value
    pthread_rwlock_unlock(&list_lock);
}

int delete(Element_t *element) {
    pthread_rwlock_wrlock(&list_lock);
    // Free the payload
    free((void *) element->value);
    // Basic checks? Why does the question need these stuff?
    if (element->prev == NULL || element->next == NULL) {
        pthread_rwlock_unlock(&list_lock);
        return 1;
    }
    // Very special case which prev and next are equal. This means
    // that this is the only element in list
    if (element->prev == element->next) {
        struct Element *head = element->prev;
        head->prev = NULL;
        head->next = NULL;
        free(element);
        pthread_rwlock_unlock(&list_lock);
        return 0;
    }
    // Otherwise, just join next and prev
    struct Element *prev = element->prev;
    struct Element *next = element->next;
    prev->next = next;
    next->prev = prev;
    free(element);
    pthread_rwlock_unlock(&list_lock);
    return 0;
}

Element_t *lookup(LinkedList_t *list, const char *value) {
    pthread_rwlock_rdlock(&list_lock);
    // Check for empty list
    if (list->next == NULL) {
        pthread_rwlock_unlock(&list_lock);
        return NULL;
    }
    // Iterate over list
    Element_t *elem = list->next;
    while (elem->value != NULL) {
        if (strcmp(value, elem->value) == 0) {
            pthread_rwlock_unlock(&list_lock);
            return elem;
        }
        elem = elem->next;
    }
    pthread_rwlock_unlock(&list_lock);
    return NULL;
}

int get_length(LinkedList_t *list) {
    pthread_rwlock_rdlock(&list_lock);
    // Check for empty list
    if (list->next == NULL) {
        pthread_rwlock_unlock(&list_lock);
        return 0;
    }
    // Iterate and accumulate
    int size = 0;
    Element_t *elem = list->next;
    while (elem->value != NULL) {
        size++;
        elem = elem->next;
    }
    pthread_rwlock_unlock(&list_lock);
    return size;
}