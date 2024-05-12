#include <stdio.h>
#include <stdint.h>

// Set, Clear and Check bits across integer boundaries
// i.e. given an array int arry[10]; which is 10*32 bits wide, 
// set individual bits across the integer boundaries with the macros below

#define SETBIT(A, n) ( A[n/32] |= (1 << n%32) )
#define CLEARBIT(A, n) ( A[n/32] &= ~(1 << n%32) )
#define CHECKBIT(A, n) ( A[n/32] & (1 << n%32) )
// n/32 to determine the integer index, i.e. the 32 bit wide address to index into
// n%32 to grab the bit position within the integer address

#define ARR_SIZE 10

int main(void) {
    
    int arr[ARR_SIZE] = {0}; // 10*32bits wide
    int i;

    printf("Set the 42nd, 100th, and 142nd bits\n");
    SETBIT(arr, 42);
    SETBIT(arr, 100);
    SETBIT(arr, 142);

    // Check Set Bit
    for (i = 0; i < 32*ARR_SIZE; i++) {
        if (CHECKBIT(arr, i))
            printf("Bit %d was set\n", i);
    }

    // Clear bits
    printf("\nClearing bits\n");
    CLEARBIT(arr, 42);
    CLEARBIT(arr, 100);
    CLEARBIT(arr, 142);

    // Check
    int count = 0;
    for (i = 0; i < 32*ARR_SIZE; i++) {
        if (CHECKBIT(arr, i))
            printf("Bit %d was set\n", i);
            count++;
    }
    
    count ? printf("No bits set\n") : 0;

    return 0;
}
