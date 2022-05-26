#include "header.h"

word address;
char lines[16][80];
word breakpoints[1000];
int  numBreakpoints;
int  tracing;
byte traps[256];
unsigned int clocks;

void Cls() {
  printf("\e[H\e[2J");
  }

void GotoXY(int x, int y) {
  printf("\e[%d;%dH",y,x);
  }

int IsHex(char* line) {
  if (*line >= '0' && *line <= '9') return -1;
  if (*line >= 'a' && *line <= 'f') return -1;
  if (*line >= 'A' && *line <= 'F') return -1;
  return 0;
  }

word GetHex(char* line) {
  word ret;
  ret = 0;
  while (IsHex(line)) {
    ret <<= 4;
    if (*line >= '0' && *line <= '9') ret |= (*line - '0');
    if (*line >= 'a' && *line <= 'f') ret |= (*line - 87);
    if (*line >= 'A' && *line <= 'F') ret |= (*line - 55);
    line++;
    }
  return ret;
  }

void DrawScreen() {
  Cls();
  printf("+-A--+   +--------------------------------------------------------------------+\n");
  printf("| -- |   |                                                                    |\n");
  printf("+----+   |                                                                    |\n");
  printf("+-X--+   |                                                                    |\n");
  printf("| -- |   |                                                                    |\n");
  printf("+----+   |                                                                    |\n");
  printf("+-Y--+   |                                                                    |\n");
  printf("| -- |   |                                                                    |\n");
  printf("+----+   |                                                                    |\n");
  printf("+-S--+   |                                                                    |\n");
  printf("| -- |   |                                                                    |\n");
  printf("+----+   |                                                                    |\n");
  printf("+-P--+   |                                                                    |\n");
  printf("| -- |   |                                                                    |\n");
  printf("+----+   |                                                                    |\n");
  printf("+--PC--+ |                                                                    |\n");
  printf("| ---- | |                                                                    |\n");
  printf("+------+ +--------------------------------------------------------------------+\n");
  printf("- - - - - - -\n");
  printf("\n");
  printf("  -->\n");
  printf("    >");
  }

void UpdateScreen() {
  GotoXY(3,2); printf("%02x",cpu.a);
  GotoXY(3,5); printf("%02x",cpu.x);
  GotoXY(3,8); printf("%02x",cpu.y);
  GotoXY(3,11); printf("%02x",cpu.s);
  GotoXY(3,14); printf("%02x",cpu.p);
  GotoXY(3,17); printf("%04x",cpu.pc);
  GotoXY(1,19); if (cpu.p & FLAG_N) printf("N"); else printf("-");
  GotoXY(3,19); if (cpu.p & FLAG_V) printf("V"); else printf("-");
  GotoXY(5,19); if (cpu.p & FLAG_B) printf("B"); else printf("-");
  GotoXY(7,19); if (cpu.p & FLAG_D) printf("D"); else printf("-");
  GotoXY(9,19); if (cpu.p & FLAG_I) printf("I"); else printf("-");
  GotoXY(11,19); if (cpu.p & FLAG_Z) printf("Z"); else printf("-");
  GotoXY(13,19); if (cpu.p & FLAG_C) printf("C"); else printf("-");
  GotoXY(30,19); printf("Scanline: %03d  Dot Clock: %03d",tia.ScanLine, tia.DotClock);
  }

void Output(char* msg) {
  int i;
  for (i=0; i<15; i++) {
    strcpy(lines[i], lines[i+1]);
    GotoXY(11,i+2); printf("%s", lines[i]);
    }
  strcpy(lines[15], msg);
  while (strlen(lines[15]) < 68) strcat(lines[15]," ");
  lines[15][68] = 0;
  GotoXY(11,17); printf("%s",lines[15]); fflush(stdout);
  }

void debugger_qm(char* line) {
  char l[70];
  char t[32];
  int  stack;
  int  x,y;
  byte b;
  stack = 0;
  if (*line == 'c' || *line == 'C') {
    sprintf(l,"Clocks: %u",clocks);
    Output(l);
    line++;
    if (*line == '-') clocks = 0;
    return;
    }
  if (*line == 't' || *line == 'T') {
    sprintf(l," VSYNC:%02x  VBLANK:%02x",tia.vsync,tia.vblank);
    Output(l);
    sprintf(l,"  GRP0:%02x  COLUP0:%02x  POSP0:%02x  HMP0:%02x  REFP0:%02x  ",
      tia.grp0, tia.colorp0, tia.posp0, tia.hmp0, tia.refp0);
    strcpy(t,"");
    b = tia.grp0;
    for (x=0; x<8; x++) {
      if (b & 0x80) strcat(t,"#"); else strcat(t,"_");
      b <<= 1;
      }
    strcat(l, t);
    Output(l);
    sprintf(l,"  GRP1:%02x  COLUP1:%02x  POSP1:%02x  HMP1:%02x  REFP1:%02x  ",
      tia.grp1, tia.colorp1, tia.posp1, tia.hmp1, tia.refp1);
    strcpy(t,"");
    b = tia.grp1;
    for (x=0; x<8; x++) {
      if (b & 0x80) strcat(t,"#"); else strcat(t,"_");
      b <<= 1;
      }
    strcat(l, t);
    Output(l);
    sprintf(l," ENAM0:%02x  RESMP0:%02x  POSM0:%02x  HMM0:%02x",
      tia.enam0, tia.resmp0, tia.posm0, tia.hmm0);
    Output(l);
    sprintf(l," ENAM1:%02x  RESMP1:%02x  POSM1:%02x  HMM1:%02x",
      tia.enam1, tia.resmp1, tia.posm1, tia.hmm1);
    Output(l);
    sprintf(l," ENABL:%02x             POSBL:%02x  HMBL:%02x",
      tia.enabl, tia.posbl, tia.hmbl);
    Output(l);
    sprintf(l,"NUSIZ0:%02x  NUSIZ1:%02x",tia.nusiz0, tia.nusiz1);
    Output(l);
    sprintf(l,"COLUBK:%02x  COLUPF:%02x  CTRLPF:%02x  ",
      tia.background << 1, tia.pfForeground << 1, tia.ctrlpf);
    strcpy(t,"");
    y = tia.playfield;
    for (x=0; x<20; x++) {
      if (y & 0x80000) strcat(t,"#"); else strcat(t,"_");
      y <<= 1;
      }
    strcat(l, t);
    Output(l);
    sprintf(l," CXM0P:%02x   CXM1P:%02x  CXP0FB:%02x  CXP1FB:%02x",
      tia.cxm0p, tia.cxm1p, tia.cxp0fb, tia.cxp1fb);
    Output(l);
    sprintf(l,"CXM0FB:%02x  CXM1FB:%02x  CXBLPF:%02x  CXPPMM:%02x",
      tia.cxm0fb, tia.cxm1fb, tia.cxblpf, tia.cxppmm);
    Output(l);
    sprintf(l," INPT4:%02x   INPT5:%02x", tia.inpt4, tia.inpt5);
    Output(l);
    return;
    }
  if ((*line == 'p' || *line == 'P') &&
      (*(line+1) == 'i' || *(line+1) == 'I')) {
    sprintf(l,"TINT:%2x  TIMER:%02x  SWCHA:%02x  SWCHB:%02x",
      pia.tint, pia.timer, pia.swcha, pia.swchb);
    Output(l);
    return;
    }
  if (IsHex(line)) address = GetHex(line);
  else if (*line == 'p' || *line == 'P') address = cpu.pc;
  else if (*line == 's' || *line == 'S') {
    address = cpu.s;
    stack = -1;
    }
  for (y=0; y<16; y++) {
    if (stack) sprintf(l,"%04x:",0x100 + (address & 0xff));
      else sprintf(l,"%04x:",address);
    for (x=0; x<16; x++) {
      if (stack) sprintf(t," %02x",ram[0x100 + ((address++) & 0xff)]);
        else sprintf(t," %02x",ram[(address++) & 0x1fff]);
      strcat(l, t);
      }
    Output(l);
    }
  }

void debugger_dl(char* line) {
  char l[80];
  int  x,y;
  if (IsHex(line)) address = GetHex(line);
  else if (*line == 'p' || *line == 'P') address = cpu.pc;
  for (y=0; y<16; y++) {
    address = Disassem(address, l);
    Output(l);
    }
  }

void debugger_ex(char*line) {
  word v;
  word addr;
  if (*line == 'P' || *line == 'p') addr = cpu.pc;
  else if (IsHex(line)) {
    addr = GetHex(line);
    while (IsHex(line)) line++;
    while (*line == ' ') line++;
    }
  else return;
  while (IsHex(line)) {
    v = GetHex(line);
    ram[addr++] = v;
    while (IsHex(line)) line++;
    while (*line == ' ') line++;
    }
  }

void debugger_a(char*line) {
  if (*line == '=') {
    line++;
    if (IsHex(line)) {
      cpu.a = GetHex(line);
      UpdateScreen();
      }
    }
  }

void debugger_b(char* line) {
  int  i,j;
  word addr;
  char l[70];
  char t[6];
  if (*line == '?') {
    Output("Breakpoints:");
    i = 0;
    strcpy(l,"  ");
    while (i < numBreakpoints) {
      sprintf(t," %04x", breakpoints[i++]);
      strcat(l, t);
      if (strlen(l) > 62) {
        Output(l);
        strcpy(l,"  ");
        }
      }
    if (strlen(l) > 2) Output(l);
    return;
    }
  if (*line == '+') {
    line++;
    addr = GetHex(line);
    for (i=0; i<numBreakpoints; i++)
      if (breakpoints[i] == addr) return;
    breakpoints[numBreakpoints++] = addr;
    return;
    }
  if (*line == '-') {
    line++;
    addr = GetHex(line);
    for (i=0; i<numBreakpoints; i++)
      if (breakpoints[i] == addr) {
        for (j=i; j<numBreakpoints-1; j++)
          breakpoints[j] = breakpoints[j+1];
        numBreakpoints--;
        return;
        }
    return;
    }
  if (*line == 'c' || *line == 'C') {
    numBreakpoints = 0;
    return;
    }
  }

void debugger_p(char*line) {
  if (*line == '=') {
    line++;
    if (IsHex(line)) {
      cpu.p = GetHex(line);
      UpdateScreen();
      }
    return;
    }
  if (*line == 'c' || *line == 'C') {
    line++;
    if (*line == '=') {
      line++;
      if (IsHex(line)) {
        cpu.pc = GetHex(line);
        UpdateScreen();
        }
      }
    return;
    }
  }

void debugger_s(char*line) {
  if (*line == '=') {
    line++;
    if (IsHex(line)) {
      cpu.s = GetHex(line);
      UpdateScreen();
      }
    }
  }

void debugger_t(char* line) {
  int  i,j;
  word addr;
  char l[70];
  char t[6];
  if (*line == 'r' || *line == 'R') {
    line++;
    if (*line == '+') tracing = 0xff;
    if (*line == '-') tracing = 0x00;
    return;
    }
  if (*line == '?') {
    Output("Traps:");
    i = 0;
    strcpy(l,"  ");
    for (i=0; i<256; i++) {
      if (traps[i] != 0) {
        sprintf(t," %02x", i);
        strcat(l, t);
        if (strlen(l) > 62) {
          Output(l);
          strcpy(l,"  ");
          }
        }
      }
    if (strlen(l) > 2) Output(l);
    return;
    }
  if (*line == '+') {
    line++;
    addr = GetHex(line);
    traps[addr & 0xff] = 0xff;
    return;
    }
  if (*line == '-') {
    line++;
    addr = GetHex(line);
    traps[addr & 0xff] = 0x00;
    return;
    }
  if (*line == 'c' || *line == 'C') {
    for (i=0; i<256; i++) traps[i] = 0x00;
    return;
    }
  }

void debugger_x(char*line) {
  if (*line == '=') {
    line++;
    if (IsHex(line)) {
      cpu.x = GetHex(line);
      UpdateScreen();
      }
    }
  }

void debugger_y(char*line) {
  if (*line == '=') {
    line++;
    if (IsHex(line)) {
      cpu.y = GetHex(line);
      UpdateScreen();
      }
    }
  }

void debugger_step() {
  c6502_cycle(&cpu);
  dbg_clocks += cpu.clocks;
  cpu.clocks *= 3;
  while (cpu.clocks != 0) {
    tia_cycle(&tia);
    pia_cycle(&pia);
    cpu.clocks--;
    }
  if (pending_tia) {
    tia_write(&tia, pending_port, pending_value);
    pending_tia = 0;
    }
  while (cpu.idle != 0) {
    tia_cycle(&tia);
    pia_cycle(&pia);
    }
  }

void debugger_run(char* line) {
  int i;
  char dis[1024];
  run = -1;
  if (IsHex(line)) cpu.pc = GetHex(line);
  while (run) {
    if (tracing) {
      Disassem(cpu.pc, dis);
      Output(dis);
      }
    debugger_step();
    for (i=0; i<numBreakpoints; i++)
      if (cpu.pc == breakpoints[i]) {
        run = 0;
        UpdateScreen();
        }
    if (traps[ram[cpu.pc]] != 0) {
      run = 0;
      UpdateScreen();
      }
    }
  }

void Debugger() {
  int i;
  int flag;
  char line[1024];
  char dis[1024];
  for (i=0; i<15; i++)
    strcpy(lines[i],"                                                                    ");
  for (i=0; i<256; i++) traps[i] = 0;
  address = 0;
  numBreakpoints = 0;
  tracing = 0;
  clocks = 0;
  DrawScreen();
  UpdateScreen();
  flag = -1;
  while (flag) {
    GotoXY(6, 21); printf("                                                               ");
    GotoXY(6, 22); printf("                                                               ");
    Disassem(cpu.pc, line);
    GotoXY(6, 21); printf("%s",line);
    GotoXY(1, 22); printf("%04x>",cpu.pc);
    fgets(line, 1023, stdin);
    while (strlen(line) > 0 && line[strlen(line)-1] <= ' ')
      line[strlen(line)-1] = 0;
    if (strcmp(line,"/") == 0) flag = 0;
    if (line[0] == '?') debugger_qm(line+1);
    if (line[0] == '$') debugger_dl(line+1);
    if (line[0] == '!') debugger_ex(line+1);
    if (line[0] == 'b' || line[0] == 'B') debugger_b(line+1);
    if (line[0] == 't' || line[0] == 'T') debugger_t(line+1);
    if (line[0] == 'a' || line[0] == 'A') debugger_a(line+1);
    if (line[0] == 's' || line[0] == 'S') debugger_s(line+1);
    if (line[0] == 'x' || line[0] == 'X') debugger_x(line+1);
    if (line[0] == 'y' || line[0] == 'Y') debugger_y(line+1);
    if (line[0] == 'p' || line[0] == 'P') debugger_p(line+1);
    if (line[0] == '@') debugger_run(line+1);
    if (line[0] == 'i' || line[0] == 'I') c6502_irq(&cpu);
    if (line[0] == 'n' || line[0] == 'N') c6502_nmi(&cpu);
    if (line[0] == 'r' || line[0] == 'R') {
      c6502_reset(&cpu);
      UpdateScreen();
      }
    if (strlen(line) == 0) {
      if (tracing) {
        Disassem(cpu.pc, dis);
        Output(dis);
        }
      debugger_step();
      UpdateScreen();
      }
    }
  }



