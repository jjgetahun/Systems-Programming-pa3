#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

static char smallBlock[1000]; /*For small allocation requests*/
static char bigBlock[4000]; /*For large allocation requests*/
static void* myMemList[5000]; //To keep track of malloc address locations

/*
 * Initializes the myMemList array
 */
void initalizeList() {
    int i = 0;
    for (; i < 5000; i++) {
        myMemList[i] = NULL;
    }
    return;
}

/*
 * Stores the argument, which is a void* ptr, in the indexes of the
 * myMemList array where the value is NULL
 */
void assignPtrSpace(void* ptr) {
    int i = 0;
    for (; i < 5000; i++) {
        if(myMemList[i] == NULL) {
            myMemList[i] = ptr;
            return;
        }
    }
    return;
}

/*
 * Checks if there is a void* ptr at each index of the myMemList array,
 * setting the value of that index to NULL and returning 1, if so
 */
int isAddrValid(void* ptr) {
    int i = 0;
    for (; i < 5000; i++) {
        if(myMemList[i] == ptr) {
            myMemList[i] = NULL;
            return 1;
        }
    }
    return 0;
}

/*
 * Allocates space for requests greater than 100 bytes, if possible
 */
void * bigmalloc(unsigned int size, char * file, int line) {
    static int initBig = 0;
    static memEntry * root;
    memEntry * p, * succ;

    if (!initBig) {
        root = (memEntry *)bigBlock;
        root->prev = root->succ = 0;
        root->size = 4000-sizeof(memEntry);
        root->isFree = 1;
        initBig = 1;
    }

    p = root;

    while (p != 0) {
        if (p->size < size)
            p = p->succ;
        else if (p->isFree != 1)
            p = p->succ;
        else if (p->size < (size+sizeof(memEntry)+20)) {
            p->isFree = 0;
            assignPtrSpace((void*) p+sizeof(memEntry));
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
            assignPtrSpace((void*) p+sizeof(memEntry));
            return (char *)p+sizeof(memEntry);
        }
    }
    fprintf(stderr, "No available space. Error caused by line %d in %s.\n", line, file);
    return 0;
}

/*
 * Allocates space for requests that are 100 bytes or smaller,
 * if possible
 */
void * smallmalloc(unsigned int size, char * file, int line) {
    static int initSmall = 0;
    static memEntry * root;
    memEntry * p, * succ;

    if (!initSmall) {
        root = (memEntry *)smallBlock;
        root->prev = root->succ = 0;
        root->size = 1000-sizeof(memEntry);
        root->isFree = 1;
        initSmall = 1;
    }

    p = root;

    while (p != 0) {
        if (p->size < size)
            p = p->succ;
        else if (p->isFree != 1)
            p = p->succ;
        else if (p->size < (size+sizeof(memEntry)+1)) {
            p->isFree = 0;
            assignPtrSpace((void*) p+sizeof(memEntry));
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
            assignPtrSpace((void*) p+sizeof(memEntry));
            return (char *)p+sizeof(memEntry);
        }
    }
    fprintf(stderr, "No available space. Error caused by line %d in %s.\n", line, file);
    return 0;
}

/*
 * Initializes the myMemList array if it has not already been
 * initialized, and determines whether bigmalloc() or smallmalloc()
 * should be called (based on the size of the request)
 */
void* mymalloc (unsigned int size, char * file, int line) {
    static int initList = 0;
    if (!initList) {
        initalizeList();
        initList = 1;
    }
    if (size > 100) {
        return bigmalloc(size, file, line);
    }
    else {
        return smallmalloc(size, file, line);
    }
}

/*
 * Frees memory that has been allocated, if possible
 */
void myfree (void *p, char * file, int line) {
    memEntry *ptr, *pred, *succ;

    if (p == NULL) {
        fprintf(stderr, "Cannot free NULL pointer. Error caused by line %d in %s.\n", line, file);
        return;
    }

    if (!isAddrValid((void*)p)) {
        fprintf(stderr, "Address passed was not returned from malloc() or has already been freed. Error caused by line %d in %s.\n", line, file);
        return;
    }

    ptr = (memEntry *)((char *)p - sizeof(memEntry));
    pred = ptr->prev;
    if (pred != 0 && pred->isFree == 1) {
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

/*
 * Checks every index of the myMemList array to see if all of
 * the malloced space was freed
 */
void chkMallocSpace() {
    int i=0;
    for (; i < 5000; i++) {
        if(myMemList[i] != NULL) {
            fprintf(stderr,"Not all malloced space was freed.\n");
            return;
        }
    }
    return;
}
