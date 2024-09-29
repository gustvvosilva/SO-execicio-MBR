#include "main.h"  // Header do nosso código.

int main() {

    FILE *file = fopen("mbr.bin", "rb");  // Abre o arquivo em modo de leitura de bytes.
    __uint8_t bytes[512];  // Array de 512 bytes.

    fread(bytes, 1, 512, file);  // Pega todos os 512 bytes da MBR.
    fclose(file);  // Retira o arquivo da memória, visto que já fizemos a extração dos dados.

    // printf("\nHEXDUMP:");
    // for(int i = 0; i < 512; i++) {

    //     if (i % 2 == 0)
    //         printf(" ");
    //     if (i % 16 == 0)
    //         printf("\n%07x ", i);

    //     printf("%02x", bytes[i]);
    // }
    // printf("\n");

    DEV device[MAX_DEVS];  // Faz um array de devices para receber os valores de cada um separado.
    int qtd_particoes,     // Quantidade de partições encontradas na MBR. Serve pra quando for imprimi-las.
        pl;                // PL significa PULA LINHA, ou seja, toda vez que for feito uma iteração no laço
                           // de atribuição abaixo, ele vai pegar as informações da próxima partição.
    __uint32_t all_sectors = 0;  // Soma todos os setores do disco.

    for(int i = 0; i < MAX_DEVS; i++) {

        pl = 16 * i;  // Cada linha tem 16 bytes de dados, e também cada partição tem 16 bytes.

        // Busca em qual setor começa a partição que ele está extraindo.
        // Como o MBR se encontra no formato Little Endian, precisamos obter as informações invertendo os bytes.
        // Além disso, também fiz o LEFT SHIFT para guardar 4 bytes em uma variável.
        // Nesse laço, realizei a busca dos bytes passando um inteiro como index,
        // mas também é possivel fazer essa busca usando hexadecimal, deixando a busca na MBR parecido com
        // o que o comando HEXDUMP faz. Deixei um exemplo disso mais a frente no código.
        device[i].start = (bytes[457 + pl] << 24) + (bytes[456 + pl] << 16) + (bytes[455 + pl] << 8) + (bytes[454 + pl]);

        // Se não for encontrado o start, significa que não existem mais partições a serem lidas.
        if (device[i].start == 0) {
            qtd_particoes = i;
            break;
        }

        device[i].num_device = i + 1;

        // Verifica se o espaço 446 (0x1BE) tem o valor 128 (0x080),
        // se sim, atribui asterístico para dizer que é bootavel, se não, atribui espaço vazio.
        device[i].bootable = bytes[446 + pl] == 128 ? '*' : 32;
        device[i].sectors = (bytes[461 + pl] << 24) + (bytes[460 + pl] << 16) + (bytes[459 + pl] << 8) + (bytes[458 + pl]);
        device[i].end = device[i].start + device[i].sectors - 1;

        // Como cada setor tem 512 bytes, fazendo essas divisões deixei o SIZE em GigaBytes.
        device[i].size = (((device[i].sectors / 2.0) / 1024) / 1024);
        device[i].id = bytes[450 + pl];

        // Obtem a quantidade setores de todas as partições combinadas.
        if (all_sectors == 0)
            all_sectors = device[i].start + device[i].sectors;
        else
            all_sectors += device[i].sectors;
    }

    __uint64_t all_bytes = ((__uint64_t)all_sectors * 512);  // Quantidade total de bytes do disco.
    double all_size = (((all_sectors / 2.0) / 1024) / 1024);  // Tamanho em GigaBytes do disco.

    // Identificador único do disco.
    // Aqui mostrei como podemos buscar os bytes passando o index em hexadecimal.
    __uint32_t identifier = (bytes[0x1bb] << 24) + (bytes[0x1ba] << 16) + (bytes[0x1b9] << 8) + (bytes[0x1b8]);

    // Imprime as informações obtidas igual* ao comando "fdisk -l /dev/sda".
    // *(Ocultei algumas linhas desse comando pois seriam apenas um copia-cola estático, 
    // apenas mostrei as linhas importantes e com conteúdo dinâmico).
    printf("\nDisk /dev/sda: %.0lf GiB, %ld bytes, %d sectors\n", all_size, all_bytes, all_sectors);
    printf("Disk identifier: 0x%x\n", identifier);

    printf("\nDevice     Boot   Start      End  Sectors Size Id Type\n");
    for(int i = 0; i < qtd_particoes; i++) {
        printf("/dev/sda%d  %c    %7d %8d %8d %3.0lfG %2x %s\n", 
            device[i].num_device, device[i].bootable, device[i].start, 
            device[i].end, device[i].sectors, device[i].size, 
            device[i].id, tipos(device[i].id));
    }

    return 0;
}

// Tipos conhecidos para nosso MBR.
// Caso seja utilizado outro MBR, os tipos não conhecidos retornaram NOT FOUND.
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