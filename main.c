#include <stdio.h>
#include <string.h>

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

    __uint32_t combinado1 = ((long)bytes[461] << 24) + ((long)bytes[460] << 16) + ((long)bytes[459] << 8) + ((long)bytes[458]);
    __uint32_t combinado2 = ((long)bytes[477] << 24) + ((long)bytes[476] << 16) + ((long)bytes[475] << 8) + ((long)bytes[474]);

    printf("%d - %d\n", combinado1, combinado2);

    printf("/dev/sda%d   %ck\n",
     1, 
     bytes[446] == 128 ? '*' : '\0');

    int device;
    __uint32_t start, end, sectors;
    double size;
    __uint8_t id;
    char bootable, type[10];
    printf("\nDevice    Boot Start End Sectors Size Id Type\n");

    for(int i = 0; i < 2; i++) {

        device = i + 1;
        bootable = bytes[446 + (16 * i)] == 128 ? '*' : 32;
        start = ((long)bytes[457 + (16 * i)] << 24) + ((long)bytes[456 + (16 * i)] << 16) + ((long)bytes[455 + (16 * i)] << 8) + ((long)bytes[454 + (16 * i)]);
        sectors = ((long)bytes[461 + (16 * i)] << 24) + ((long)bytes[460 + (16 * i)] << 16) + ((long)bytes[459 + (16 * i)] << 8) + ((long)bytes[458 + (16 * i)]);
        end = start + sectors - 1;
        size = (((sectors / 2.0) / 1024) / 1024);
        id = bytes[450 + (16 * i)];

        printf("/dev/sda%d %c    %7d %8d %8d %3.0lfG %2x\n", device, bootable, start, end, sectors, size, id);





    }

    fclose(file);
    return 0;
}