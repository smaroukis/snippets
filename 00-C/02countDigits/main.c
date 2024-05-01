#include <stdio.h>
#include <stdint.h>

typedef int (*generalTestFunct)(); // change return type and arguments as nec.

#define ENDPOS 3
// Returns starting position for an LED display based on input
// i.e. 9999 => 0; 999 => 1 ... 9 => 3
int countDigitsReturnPos(int num)
{
    printf("\n");
    printf("Input: %d\n", num);
    num /= 10;
    int i = ENDPOS;
    while (num != 0) {
        num /= 10;
        i--;
    }
    printf("Output: %d\n", i);
    printf("\n");
    return i;
}

int main(void) {

    generalTestFunct test_func = &countDigitsReturnPos;
    
    // test 1:
    test_func(9999);
    test_func(999);
    test_func(99);
    test_func(9);
    
    // test 2:
    
    // test 3:
    
    // test 4:

    return 0;
}
