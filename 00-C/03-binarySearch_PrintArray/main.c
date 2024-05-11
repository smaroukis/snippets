// From Ritchie 1978 pp54
// Implement a binary search function that decides if a particular value x occurs in the sorted array v. 
// The elements of v must already be in increasing order
// The function returns the position if x occurs in v and -1 if not
// This code demonstrates a three way decision (if, elseif else)

// Print Array
// Key is to create a pointer at the beginning of the string and incremement and assign e.g. p += sprintf(p, "%d", arr[i])

// CANNOT re initialize an array using the assigment after it has been declared e.g. int arr[] = {1, 2}; ... arr[] = {4, 5} // compile error
// → use mcmcpy

#include <stdio.h>
#include <stdlib.h> // doesn't have itoa?
#include <stdint.h>
#include <string.h> // for memcpy for array re-assigment
// ? include lib for itoa?

// Decls
typedef int (*generalTestFunct)(int, int*, int); // change return type and arguments as nec.
void printArr(int arr[], int len);
char *itoa(int value, char *str);

// Defs
int BinarySearch(int x, int arr[], int len)
{
    // Use two extra indices to keep track of high and low, move as required based on comparison with x and arr[i]
    int high = len - 1; // start at the last element
    int low = 0;
    int i;

    // Take the last case of low = high, then 
    while (low <= high) {
        // midpoint is average
        i = (high + low) / 2;
        if (x > arr[i]) {
            low = i + 1;
        }
        else if (x < arr[i]) {
            high = i - 1;
        }
        else {
            // match found
            printf("Found %d in ", x);
            printArr(arr, len);
            return i;
        }
    }

    printf("Did not find %d in ", x);
    printArr(arr, len);
    return -1;
}

char *itoa(int value, char *str) {
    char *start = str;
    int base = 10; // add default base
    
    // Handle negative numbers
    if (value < 0) {
        *str++ = '-';
        value = -value;
    }
    
    // Convert to string in reverse order
    do {
        *str++ = "0123456789abcdef"[value % base];
    } while (value /= base);
    
    *str-- = '\0';
    
    // Reverse the string
    while (start < str) {
        char temp = *start;
        *start++ = *str;
        *str-- = temp;
    }
    
    return start;
}

void printArr(int arr[], int len) {
    char str[1024]; 
    char *p = str;
    p += sprintf(p, "{ "); // better than assigning and incrementing pointers
    
    for (int i = 0; i < len; i++) {
        p += sprintf(p, "%d", arr[i]); // store char at current p address and increment p
        if (i < len - 1) {
            p += sprintf(p, ", "); 
        }
    }
    sprintf(p, " }");
    printf("%s\n", str);
}

int main(void) {

    generalTestFunct test_func = &BinarySearch;
    
    // test printarray
    printf("Testing printArray()\n"); 
    int arr[] = {1, 2, 3};
    printArr(arr, sizeof(arr)/sizeof(arr[0]));
    
    // test 1:
    int x = 3;
    const int inits[] = {4, 5, 6};
    memcpy(arr, inits, sizeof(inits));
    test_func(x, arr, sizeof(arr)/sizeof(arr[0]) );

    // test 2:
    x = 6;
    int arr3[] = {1, 2, 3};
    test_func(x, arr3, sizeof(arr3)/sizeof(arr3[0]) );

    x = -5;
    int arr4[] = {-5, 0, 2, 5};
    test_func(x, arr4, sizeof(arr4)/sizeof(arr4[0]) );

    
    // test 3:
    
    // test 4:

    return 0;
}
