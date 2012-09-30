#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
typedef unsigned char BYTE;

typedef struct entry {
    TAILQ_ENTRY(entry)    entries;
    int len;
    BYTE *data;
}St_Entry;
TAILQ_HEAD(, entry) head;


int main(void)
{
    St_Entry *n;
    int i;
	TAILQ_INIT(&head);
    /* Push nodes */
    for(i=0; i<10; i++) {
        n = malloc(sizeof(St_Entry));
        TAILQ_INSERT_TAIL(&head, n, entries);
        n->len  = i;
        n->data = malloc(n->len);
    }

//    /* Foreach Method 1 */
//    for(n=TAILQ_FIRST(&head); n!=NULL; n=TAILQ_NEXT(n, entries)) {
//        printf("id = %d\n", n->len);
//    }

    /* Foreach Method 2 */
    TAILQ_FOREACH(n, &head, entries) {
        printf("id = %d\n", n->len);
    }

    // Pop and delete all nodes
    while((n = ((&head)->tqh_first)) != NULL) {
        TAILQ_REMOVE(&head,n, entries);
       
        free(n->data);
        free(n);
    }
    return 0;
}