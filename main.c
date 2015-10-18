#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

#define malloc( x ) mymalloc( x, __FILE__ , __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )

int main () {
    int x = 2; 
    printf("hi\n");
    
    int* y = (int*) malloc (sizeof(int));
    
    //This shows malloc is working. 
    *y = 5;
    printf("%d\n",*y);
    
    //Initial free followed by 2nd free. 
    free(y);
    free(y);

    //free of something not malloced. 
    free(&x);
    
    //free of dynamic memory not from malloc
    char* p = (char *)malloc( 200 );
    free(p+10);
    
    //Legal free. 
    p = (char *)malloc( 100 );
    free( p );
    p = (char *)malloc( 100 );
    free( p );

    return 0;
}
