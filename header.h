#ifndef _HEADER_H
#define _HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cpu6502.h"
#include "tia.h"
#include "pia.h"

#ifndef _BYTE
typedef unsigned char byte;
#define _BYTE
#endif

#ifndef _WORD
typedef unsigned short word;
#endif

#ifdef MAIN
#define LINK
#else
#define LINK extern
#endif

LINK CPU6502  cpu;
LINK TIA      tia;
LINK PIA      pia;
LINK byte     ram[65536];
LINK int      run;
LINK char     runDebugger;
LINK char     power;
LINK int      dbg_clocks;
LINK int      pending_tia;
LINK word     pending_port;
LINK byte     pending_value;


extern void c6502_irq(CPU6502* cpu);
extern void c6502_nmi(CPU6502* cpu);
extern void c6502_reset(CPU6502* cpu);

extern void Debugger();
extern word Disassem(word addr, char* dest);
extern int loadFile(char* filename);

#endif
