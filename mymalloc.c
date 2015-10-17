#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

static char myBlock[5000];

void * mymalloc (unsigned int size, char * file, int line) {
    static int initialized = 0;
    static memEntry * root;
    memEntry * p, * succ;

    if (!initialized) {
        root = (memEntry *)myBlock;
        root->prev = root->succ = 0;
        root->size = 5000-sizeof(memEntry);
        root->isFree = 1;
        initialized = 1;
    }

    p = root;

    while (p != 0) {
        if (p->size < size)
            p = p->succ;
        else if (p->isFree != 1)
            p = p->succ;
        else if (p->size < (size+sizeof(memEntry)+20)) {
            p->isFree = 0;
            return (char *)p+sizeof(memEntry);
        }
        else {
            succ = (memEntry *)((char *)p+sizeof(memEntry)+size);
            succ->prev = p;
            succ->succ = p->succ;
            if (p->succ != 0)
                p->succ->prev = succ;
            p->succ = succ;
            succ->size = p->size - sizeof(memEntry) - size;
            succ->isFree = 1;
            p->size = size;
            p->isFree = 0;
            return (char *)p+sizeof(memEntry);
        }
    }
    fprintf(stderr, "Error caused by line %d in %s\n.", line, file);
    return 0;
}

void myfree (void *p, char * file, int line) {
    memEntry *ptr, *pred, *succ;
    ptr = (memEntry *)((char *)p - sizeof(memEntry));
    pred = ptr->prev;
    if (pred != 0 && pred->isFree) {
        pred->size += sizeof(memEntry)+ptr->size;
        pred->succ = ptr->succ;
        if (ptr->succ != 0)
            ptr->succ->prev = pred;
    }
    else {
        ptr->isFree = 1;
        pred = ptr;
    }
    succ = ptr->succ;
    if (succ != 0 && succ->isFree == 1) {
        pred->size += sizeof(memEntry)+succ->size;
        pred->succ = succ->succ;
        if (succ->succ != 0)
            succ->succ->prev = pred;
    }
}
