#ifndef _PIA_H
#define _PIA_H

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
  byte tint;
  byte timer;
  byte swcha;
  byte swchb;
  word divider;
  word ticks;
  byte subticks;
  } PIA;

extern void pia_cycle(PIA *tia);
extern void pia_init(PIA *tia);
extern void pia_write(PIA *tia, word address, byte value);
extern byte pia_read(PIA *tia, word address);

#endif

