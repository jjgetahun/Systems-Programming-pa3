#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

#define malloc( x ) mymalloc( x, __FILE__ , __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )

int main () {
    int i = 0;
    for (; i < 5000; i++) {
        char* y = (char*) malloc (0);
        if (y == 0) 
            break;
    }
    
    printf("This program simulates saturaation using only char malloc.\n");
    printf("Number of chars that will fit in our array: %d\n",i);

    //Setting up to check at exit of function if everythign has been freed. 
    atexit(chkMallocSpace);

    return 0;
}
