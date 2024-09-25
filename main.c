#include <stdio.h>

int main() {

    FILE *file = fopen("mbr.bin", "rb");
    __uint8_t bytes[512];

    // printf("%d", sizeof(bytes)); > 512

    fread(bytes, 1, 512, file);

    for(int i = 0; i < 512; i++) {

        if (i % 2 == 0)
            printf(" ");
        if (i % 16 == 0)
            printf("\n");

        // if (bytes[i] == '\000')
        //     printf("00");
        // else
            printf("%2x", bytes[i]);
    }
    printf("\n");

    fclose(file);
    return 0;
}