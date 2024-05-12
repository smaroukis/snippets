#include <stdio.h>
#include <stdint.h>

/*
* @brief swaps endianness of 16 bit input data in place using temp variable
*/
void reverseEndianness16(uint16_t* data)
{
    // use temp variable and pointer
    uint8_t* pDataByte = (uint8_t*)data;
    uint16_t tmp = pDataByte[0];
    pDataByte[0] = pDataByte[1];
    pDataByte[1] = tmp;
}

/*
* @brief swaps endianness of 8 bit data in place using a loop
* @note we can extend this for any length of data input
*/
void reverseEndianness(uint8_t* start, int len) {

    for (int i=0, k=len-1; i<=k; i++, k--) {
        uint8_t tmp = start[i];
        start[i] = start[k];
        start[k] = start[i];
    }
}

/*
* @brief swaps endianness of 32 bit data type using bitwise operators
*/
uint32_t reverseEndianBitwise(uint32_t data){
    // swap first and last 16 bits
    data = ((data & 0xffff0000) >> 16 | (data & 0x0000ffff) << 16);
    // swap first and second, and third and fourth bytes
    data = ((data & 0xff00ff00) >> 8 | (data & 0x00ff00ff) << 8);
    return data;
}

/*
* @brief swaps endianness in place for input data of any size with a loop 
*/
void reverseEndian3(void* data, size_t len) {
    uint8_t* start = (uint8_t*)data;
    uint8_t* end = start + len - 1;
    while (start < end) {
        uint8_t tmp = *start;
        *start++ = *end; // decrements the pointer not the pointed-to-value
        *end-- = tmp;
    }
}

int main(void) {

    // Test 16
    uint16_t data = 0x3210; // Example number to swap endianness

    printf("Before swap: 0x%04X\n", data);
    reverseEndianness16(&data);
    printf("After swap: 0x%04X\n", data);

    // Test General
    uint8_t data2[] = {0x01, 0x02, 0x03, 0x04}; // Example data2 array
    int len = sizeof(data2) / sizeof(data2[0]);

    printf("Before swap: ");
    for (int i = 0; i < len; i++) {
        printf("%02X ", data2[i]);
    }
    printf("\n");

    reverseEndianness(data2, len);

    printf("After swap: ");
    for (int i = 0; i < len; i++) {
        printf("%02X ", data2[i]);
    }
    printf("\n");

    return 0;
 
}
