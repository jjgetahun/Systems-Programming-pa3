#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

#define malloc( x ) mymalloc( x, __FILE__ , __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )

int main () {
    int x = 2;
    int* y = (int*) malloc (sizeof(int));

    //This shows malloc is working.
    printf("Malloc working.\n");
    *y = 5;
    printf("%d\n",*y);

    //Initial free followed by 2nd free.
    printf("Two frees on the same pointer.\n");
    free(y);
    free(y);

    //free of something not malloced.
    
    printf("Freeing a pointer that was not malloced.\n");
    free(&x);

    //free of dynamic memory not from malloc
    
    printf("Freeing a pointer that was not malloced by addition to a malloced pointer (p+10).\n");
    char* p = (char *)malloc( 200 );
    free(p+10);

    //Legal free.
    
    printf("Malloc, free, malloc, free. Legal.\n");
    p = (char *)malloc( 100 );
    free( p );
    p = (char *)malloc( 100 );
    free( p );
    
    //Malloc space 0
    
    printf("Test of malloc(0)\n");
    int* z = (int*) malloc(sizeof(0));
    free(z);
    
    //Setting up to check at exit of function if everythign has been freed. 
    atexit(chkMallocSpace);

    return 0;
}
