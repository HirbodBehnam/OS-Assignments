/**
 *	This structure is used for a list head, and each element
 *	of the list begins with this structure.
 *
 *  An empty list contains only a list head.  
 *  The next pointer in the head points at the first elment in the list.  
 *  The prev pointer in the list head points at the last element in the list.
 *  The list is circular.
 *  The list head is determined by its NULL value pointer.
 *
 *                     *** DO NOT MAKE ANY CHANGE TO THIS FILE! ***
 */
struct Element {
	const char *value;
	struct Element *prev;
	struct Element *next;
};
typedef struct Element LinkedList_t;
typedef struct Element Element_t;


/**
 * insert an element into a list
 *
 * @param LinkedList_t *list ... header for the list
 * @param Element_t *element ... element to be added
 */
void insert(LinkedList_t *list, Element_t *element);

/**
 * remove an element from a list
 *
 * @param Element_t *element ... element to be removed
 * @return 0: element deleted successfully, 1: corrtuped prev/next pointers
 *
 */
int delete( Element_t *element);

/**
 * search list for a value
 *
 * @param LinkedList_t *list ... header for the list
 * @param const char * value ... the desired value
 *
 * @return pointer to matching element, or NULL if none is found
 */
Element_t *lookup(LinkedList_t *list, const char *value);

/**
 * count elements in a sorted list
 *
 * @param LinkedList_t *list ... header for the list
 * @return int number of elements in list (excluding head)
 *	       -1 if the list is corrupted
 */
int get_length(LinkedList_t *list);
