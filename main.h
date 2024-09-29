#include <stdio.h>
#define MAX_DEVS 4  // O máximo de partições que podemos ter nessa MBR são 4.

// Struct para guardar as informações de cada partição.
typedef struct _DEV {
    int num_device;      // index da partição (device);
    char bootable;       // é bootavel?;
    __uint32_t start;    // em que setor começa;
    __uint32_t end;      // em que setor termina;
    __uint32_t sectors;  // quantidade de setores;
    double size;         // tamanho da partição;
    __uint8_t id;        // id da partição (para achar o tipo).
} DEV;

// Retorna uma string com o tipo da partição.
char *tipos(__uint8_t id);