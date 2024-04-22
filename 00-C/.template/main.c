#include <stdio.h>
#include <stdint.h>

typedef void (*generalTestFunct)(); // change return type and arguments as nec.

void funcToTest(void)
{
    printf("Hello, World!\n");
}

int main(void) {

    generalTestFunct test_func = &funcToTest;
    
    // test 1:
    test_func();
    
    // test 2:
    
    // test 3:
    
    // test 4:

    return 0;
}
