#ifndef MOS6502DISASSEMBLER_H
#define MOS6502DISASSEMBLER_H

#include <stdint.h>
#include <ctype.h>
#include <errno.h>

/* The 6502's 13 addressing modes */
typedef enum {
    IMMED = 0, /* Immediate */
    ABSOL, /* Absolute */
    ZEROP, /* Zero Page */
    IMPLI, /* Implied */
    INDIA, /* Indirect Absolute */
    ABSIX, /* Absolute indexed with X */
    ABSIY, /* Absolute indexed with Y */
    ZEPIX, /* Zero page indexed with X */
    ZEPIY, /* Zero page indexed with Y */
    INDIN, /* Indexed indirect (with X) */
    ININD, /* Indirect indexed (with Y) */
    RELAT, /* Relative */
    ACCUM /* Accumulator */
} addressing_mode_e;

/** Some compilers don't have EOK in errno.h */
#ifndef EOK
#define EOK 0
#endif

typedef struct opcode_s {
    uint8_t number; /* Number of the opcode */
    const char *mnemonic; /* Index in the name table */
    addressing_mode_e addressing; /* Addressing mode */
    unsigned int cycles; /* Number of cycles */
    unsigned int cycles_exceptions; /* Mask of cycle-counting exceptions */
} opcode_t;

typedef struct options_s {
    char *filename; /* Input filename */
    int nes_mode; /* 1 if NES commenting and warnings are enabled */
    int cycle_counting; /* 1 if we want cycle counting */
    int hex_output; /* 1 if hex dump output is desired at beginning of line */
    unsigned long max_num_bytes;
    uint16_t org; /* Origin of addresses */
    long offset; /* File offset to start disassembly from */
} options_t;


void disassemble(char *output, uint8_t *buffer, options_t *options, uint16_t *pc);

#endif // MOS6502DISASSEMBLER_H
