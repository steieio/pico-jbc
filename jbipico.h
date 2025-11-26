#ifndef INC_JBIPICO_H
#define INC_JBIPICO_H

// #include "jbiexprt.h"
#include <unistd.h>

typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
#define TRUE 1
#define FALSE 0


#ifdef CYCLOMOD

#define JUF2_ADDRESS   0x10F00000

#define PIN_TCK 6
#define PIN_TMS 5
#define PIN_TDI 7
#define PIN_TDO 4
#define PIN_LED 28

#else

#define JUF2_ADDRESS   0x10020000

#define PIN_TCK   18
#define PIN_TMS   19
#define PIN_TDI   16
#define PIN_TDO   17
#define PIN_LED   25

#endif

#define JBD_UF2_HDR_TAG   0x3246554A  

typedef struct _jbc_uf2_hdr {
    uint32_t tag;
    uint32_t version;
    uint32_t length;
    uint32_t location;
    uint32_t speed;
    uint32_t options;
    char action[16];
    char filename[32];
    char description[128];
} jbc_uf2_hdr_t;

void jbi_init(BOOL verbose);

int jbi_play(jbc_uf2_hdr_t *juf2, char *action);

#endif /* INC_JBIPICO_H */