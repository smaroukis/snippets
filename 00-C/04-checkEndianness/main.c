#include <stdio.h>
#include <stdint.h>


// Iterate byte by byte over pointer to object printing val
// Need a char* to iterate over data byte-by-byte
void showMemRep(char *start, int n){
    int i;
    printf("(lower addresses)\n");
    for (i=0; i < n; i++) {
        printf(" %.2x", start[i]);
        printf("\n");
    }
    printf("(higher addresses)\n");
}

int main(void) {

    int i = 0x76543210; // MSByte is 0x76, LSByte is 0x10;
    printf("0x7654321 Stored in memory as:\n"); 
    // Big endian: MSByte (0x76) at lower addresses
    // Little endian: MSByte (0x76) at higher addresses
    showMemRep((char *)&i, sizeof(i));
    return 0;
}