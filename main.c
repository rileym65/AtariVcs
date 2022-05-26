#define MAIN
#include "header.h"
#include "cpu6502.h"

byte readMem(word address) {
  if (address >= 0x100 && address <= 0x1ff) address -= 0x100;
  address &= 0x1fff;
  if (address < 0x80) {
    return tia_read(&tia, address);
    }
  if (address >= 0x280 && address <= 0x297) {
    return pia_read(&pia, address);
    }
  return ram[address];
  }

void writeMem(word address, byte value) {
  if (address >= 0x100 && address <= 0x1ff) address -= 0x100;
  address &= 0x1fff;
  if (address < 0x80) {
    pending_port = address;
    pending_value = value;
    pending_tia = 0xff;
//    tia_write(&tia, address, value);
    }
  else if (address >= 0x280 && address <= 0x297) {
    pia_write(&pia, address, value);
    }
  else {
    ram[address] = value;
    }
  }

void clearMem() {
  int i;
  for (i=0; i<65536; i++) ram[i] = 0;
  }

int main(int argc, char** argv) {
  int i;
  printf("Rc/VCS v1.0.0\n");
  printf("by Michael H. Riley\n");

  cpu.readMem = readMem;
  cpu.writeMem = writeMem;
  c6502_setup(&cpu);
  tia_init(&tia);
  pia_init(&pia);
  clearMem();
  runDebugger = 0;
  pending_tia = 0;
  for (i=1; i<argc; i++) {
    if (strcmp(argv[i], "-d") == 0) runDebugger = 0xff;
    else if (loadFile(argv[i])) {
      printf("Error: Could not load %s\n",argv[i]);
      return 1;
      }
    }
  c6502_reset(&cpu);
  power = 'Y';
  if (runDebugger) Debugger();
  else {
    while (power == 'Y') {
      tia_cycle(&tia);
      pia_cycle(&pia);
      if (cpu.clocks == 0) {
        if (pending_tia) {
          tia_write(&tia, pending_port, pending_value);
          pending_tia = 0;
          }
        while (readMem(cpu.pc) == 0xff && cpu.idle == 0) {
          cpu.pc++;
          switch (readMem(cpu.pc++)) {
            case 0x00: dbg_clocks = 0; break;
            case 0x01: printf("Clocks: %d\n",dbg_clocks); break;
            case 0x02:
                 printf("Clocks: %d\n",dbg_clocks);
                 dbg_clocks = 0;
                 break;
            case 0x03:
                 printf("Scanline: %d    DotClock: %d\n",tia.ScanLine,tia.DotClock);
                 break;
            case 0x04:
                 printf("A:%02x X:%02x Y:%02x\n",cpu.a,cpu.x,cpu.y);
                 break;
            case 0x05:
                 printf("GRP0:%02x POSP0:%02x HMP0:%02x, COLUP0:%02x\n",
                        tia.grp0, tia.posp0, tia.hmp0, tia.colorp0);
                 printf("GRP1:%02x POSP1:%02x HMP1:%02x, COLUP1:%02x\n",
                        tia.grp1, tia.posp1, tia.hmp1, tia.colorp1);
                 printf("ENABL:%02x POSBL:%02x HMBL:%02x, COLUBL:%02x\n",
                        tia.enabl, tia.posbl, tia.hmbl, tia.pfForeground);
                 printf("ENAM0:%02x POSM0:%02x HMM0:%02x, COLUM0:%02x\n",
                        tia.enam0, tia.posm0, tia.hmm0, tia.colorp0);
                 printf("ENAM1:%02x POSM1:%02x HMM1:%02x, COLUM1:%02x\n",
                        tia.enam1, tia.posm1, tia.hmm1, tia.colorp1);
                 break;
            case 0x06: printf("#%d\n",readMem(cpu.pc++)); break;
            case 0x0c: printf("\n"); break;
            case 0xff: exit(1);
            }
          }
        c6502_cycle(&cpu);
        dbg_clocks += cpu.clocks;
        cpu.clocks *= 3;
        if (cpu.clocks != 0) cpu.clocks--;
        }
      else cpu.clocks--;
      }
    }
  return 0;
  }

