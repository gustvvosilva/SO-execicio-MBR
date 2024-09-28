#include <stdio.h>

char *tipos(__uint8_t id) {
    switch (id)
    {
    case 0x0b:
        return "W95 FAT32";
    case 0x83:
        return "Linux";
    default:
        return "not found";
    }
}

int main() {

    FILE *file = fopen("mbr.bin", "rb");
    __uint8_t bytes[512];

    fread(bytes, 1, 512, file);
    fclose(file);

    // printf("\nHEXDUMP:");
    // for(int i = 0; i < 512; i++) {

    //     if (i % 2 == 0)
    //         printf(" ");
    //     if (i % 16 == 0)
    //         printf("\n%07x ", i);

    //     printf("%02x", bytes[i]);
    // }
    // printf("\n");

    int device[4], qtd_particoes;
    __uint32_t start[4], end[4], sectors[4], all_sectors = 0;
    double size[4];
    __uint8_t id[4];
    char bootable[4];

    for(int i = 0; i < 4; i++) {

        start[i] = (bytes[457 + (16 * i)] << 24) + (bytes[456 + (16 * i)] << 16) + (bytes[455 + (16 * i)] << 8) + (bytes[454 + (16 * i)]);

        if (start[i] == 0) {
            qtd_particoes = i;
            break;
        }

        device[i] = i + 1;
        bootable[i] = bytes[446 + (16 * i)] == 128 ? '*' : 32;
        sectors[i] = (bytes[461 + (16 * i)] << 24) + (bytes[460 + (16 * i)] << 16) + (bytes[459 + (16 * i)] << 8) + (bytes[458 + (16 * i)]);
        end[i] = start[i] + sectors[i] - 1;
        size[i] = (((sectors[i] / 2.0) / 1024) / 1024);
        id[i] = bytes[450 + (16 * i)];

        if (all_sectors == 0)
            all_sectors = start[i] + sectors[i];
        else
            all_sectors += sectors[i];
    }

    __uint64_t all_bytes = ((__uint64_t)all_sectors * 512);
    double all_size = (((all_sectors / 2.0) / 1024) / 1024);
    __uint32_t signature = (bytes[0x1bb] << 24) + (bytes[0x1ba] << 16) + (bytes[0x1b9] << 8) + (bytes[0x1b8]);

    printf("\nDisk /dev/sda: %.0lf GiB, %ld bytes, %d sectors\n", all_size, all_bytes, all_sectors);
    printf("Disk identifier: 0x%x\n", signature);

    printf("\nDevice     Boot   Start      End  Sectors Size Id Type\n");
    for(int i = 0; i < qtd_particoes; i++) {
        printf("/dev/sda%d  %c    %7d %8d %8d %3.0lfG %2x %s\n", device[i], bootable[i], start[i], end[i], sectors[i], size[i], id[i], tipos(id[i]));
    }

    return 0;
}