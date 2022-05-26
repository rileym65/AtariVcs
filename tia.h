#ifndef _TIA_H
#define _TIA_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#ifndef _BYTE
typedef unsigned char byte;
#define _BYTE
#endif

#ifndef _WORD
typedef unsigned short word;
#endif

Display *display;
Window   window;
GC       gc;
XImage  *image;

typedef struct {
  word ScanLine;
  byte DotClock;
  byte background;
  byte vsync;
  byte vblank;
  byte ctrlpf;
  byte pfForeground;
  byte enam0;
  byte enam1;
  byte enabl;
  byte grp0;
  byte grp1;
  int  hmbl;
  int  hmbl_i;
  byte hmp0;
  byte hmp0_i;
  byte hmp1;
  byte hmp1_i;
  byte hmm0;
  byte hmm0_i;
  byte hmm1;
  byte hmm1_i;
  byte nusiz0;
  byte nusiz1;
  byte posp0;
  byte posp1;
  byte posm0;
  byte posm1;
  byte posbl;
  byte refp0;
  byte refp1;
  byte resmp0;
  byte resmp1;
  byte colorp0;
  byte colorp1;
  byte cxm0p;
  byte cxm1p;
  byte cxp0fb;
  byte cxp1fb;
  byte cxm0fb;
  byte cxm1fb;
  byte cxblpf;
  byte cxppmm;
  byte inpt4;
  byte inpt5;
  unsigned int playfield;
  } TIA;

extern void tia_cycle(TIA *tia);
extern void tia_init(TIA *tia);
extern void tia_write(TIA *tia, byte port, byte value);
extern byte tia_read(TIA *tia, byte port);

#endif

