#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main() {
    // Assuming on a 32 bit machine

    // Smaller-than-int types are promoted to (signed) int
    // the result is also signed if it fits
    uint16_t t_now = 5;  // using smaller-than-int 
    uint32_t t_start = 251; 
    auto result = t_now - t_start; 
    // result = (int)((uint32_t)(t_now) - t_start)

    uint32_t t_now = 5;  // using smaller-than-int 
    uint32_t t_start = 251; 
    auto result = t_now - t_start; 

    return 0;
}