#ifndef mymalloc_h
#define mymalloc_h

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

//void bigfree(void *p, char * file, int line);
//void smallfree(void *p, char * file, int line);
void * bigmalloc(unsigned int size, char * file, int line);
void * smallmalloc(unsigned int size, char * file, int line);
void mymalloc(unsigned int size, char * file, int line);
void myfree (void *p, char * file, int line);

//void * mymalloc (unsigned int size, char * file, int line);
//void myfree (void *p, char * file, int line);

typedef struct mem_Entry {

    struct mem_Entry *prev, *succ;
    int isFree;
    unsigned int size;

} memEntry;

#endif
