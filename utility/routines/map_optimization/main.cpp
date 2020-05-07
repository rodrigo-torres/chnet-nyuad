#include "map.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("[!] No filename provided\n");
        return 1;
    }
    printf("File to be converted: %s\n", argv[1]);
    image_xrf img(argv[1]);
    return 0;
}
