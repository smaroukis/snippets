#include <stdio.h>
#include <stdlib.h>

int main()
{
    char* msg[] = {"Hello", "C", "World", "from", "VS Code", "and the C extension!"};
    int msg_size = sizeof(msg) / sizeof(msg[0]);

    for (int i = 0; i < msg_size; i++)
    {
        printf("%s ", msg[i]);
    }
    printf("\n");

    return 0;
}