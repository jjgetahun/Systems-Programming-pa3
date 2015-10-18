#ifndef mymalloc_h
#define mymalloc_h

void * bigmalloc(unsigned int size, char * file, int line);
void * smallmalloc(unsigned int size, char * file, int line);
void * mymalloc(unsigned int size, char * file, int line);
void  myfree (void *p, char * file, int line);

typedef struct mem_Entry {

    struct mem_Entry *prev, *succ;
    int isFree;
    unsigned int size;

} memEntry;

#endif
