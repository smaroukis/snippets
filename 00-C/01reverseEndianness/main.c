#include <stdio.h>
#include <stdint.h>
#include <assert.h>

typedef uint32_t (*generalTestFunct)(uint32_t); // change return type and arguments as nec.

uint32_t reverseEndianness(uint32_t x)
{
    uint32_t tmp = 0;
    return ((x & 0x000000FF) << 3*8) |  
            ((x & 0x0000FF00) << 1*8) | 
            ((x & 0x00FF0000) >> 1*8) | 
            ((x & 0xFF000000) >> 3*8);
    return tmp; 
}

int main(void) {

    generalTestFunct test_func = &reverseEndianness;
    
    // test 1:
    uint32_t in = 0x12345678;
    uint32_t expected = 0x78563412;
    printf("in = 0x%x\nout = 0x%x\n", in, test_func(in));
    assert(expected == test_func(in));
    
    // test 2:
    
    // test 3:
    
    // test 4:

    return 0;
}
