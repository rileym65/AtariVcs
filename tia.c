#include "header.h"
#include "tia.h"
#include <X11/Xutil.h>
#include <X11/keysymdef.h>

unsigned int tia_colors[] = {
  0x000000, 0x404040, 0x6c6c6c, 0x909090, 0xb0b0b0, 0xc8c8c8, 0xdcdcdc, 0xececec,
  0x444400, 0x646410, 0x848424, 0xa0a034, 0xb8b840, 0xd0d050, 0xe8e85c, 0xfcfc68,
  0x702800, 0x844414, 0x985c28, 0xac783c, 0xbc8c4c, 0xcca05c, 0xdcb468, 0xecc878,
  0x841800, 0x983418, 0xac5030, 0xc06848, 0xd0805c, 0xe09470, 0xeca880, 0xfcbc94,
  0x880000, 0x9c2020, 0xb03c3c, 0xc05858, 0xd07070, 0xe08888, 0xeca0a0, 0xfcb4b4,
  0x78005c, 0x8c2074, 0xa03c88, 0xb0589c, 0xc070b0, 0xd084c0, 0xdc9cd0, 0xecb0e0,
  0x480078, 0x602090, 0x783ca4, 0x8c58b8, 0xa070cc, 0xb484dc, 0xc49cec, 0xd4b0fc,
  0x140084, 0x302098, 0x4c3cac, 0x6858c0, 0x7c70d0, 0x9488e0, 0xa8a0ec, 0xbcb4fc,
  0x000088, 0x1c209c, 0x3840b0, 0x505cc0, 0x6874d0, 0x7c8ce0, 0x90a4ec, 0xa4b8fc,
  0x00187c, 0x1c3890, 0x3854a8, 0x5070bc, 0x6888cc, 0x7c9cdc, 0x90b4ec, 0xa4c8fc,
  0x002c5c, 0x1c4c78, 0x386890, 0x5084ac, 0x689cc0, 0x7cb4d4, 0x90cce8, 0xa4e0fc,
  0x003c2c, 0x1c5c48, 0x387c64, 0x509c80, 0x68b494, 0x7cd0ac, 0x90e4c0, 0xa4fcd4,
  0x003c00, 0x205c20, 0x407c40, 0x5c9c5c, 0x74b474, 0x8cd08c, 0xa4e4a4, 0xb8fcb8,
  0x143800, 0x345c1c, 0x507c38, 0x6c9850, 0x84b468, 0x9ccc7c, 0xb4e490, 0xc8fca4,
  0x2c3000, 0x4c501c, 0x687034, 0x848c4c, 0x9ca864, 0xb4c078, 0xccd488, 0xe0ec9c,
  0x442800, 0x644818, 0x846830, 0xa08444, 0xb89c58, 0xd0b46c, 0xd0b46c, 0xfce08c
  };

int tia_convertOffset(byte value) {
  value >>= 4;
  switch (value) {
    case 0x07: value = -7; break;
    case 0x06: value = -6; break;
    case 0x05: value = -5; break;
    case 0x04: value = -4; break;
    case 0x03: value = -3; break;
    case 0x02: value = -2; break;
    case 0x01: value = -1; break;
    case 0x00: value =  0; break;
    case 0x0f: value = 1; break;
    case 0x0e: value = 2; break;
    case 0x0d: value = 3; break;
    case 0x0c: value = 4; break;
    case 0x0b: value = 5; break;
    case 0x0a: value = 6; break;
    case 0x09: value = 7; break;
    case 0x08: value = 8; break;
    }
  return value;
  }

void tia_init(TIA *tia) {
  int depth;
  char *data;
  XGCValues values;
  XSetWindowAttributes attr;
  XWindowAttributes    winattr;
  unsigned long        mask;
  int                  bytesrow;
  int                  bytespixel;
  display = XOpenDisplay(":0.0");
  attr.background_pixel = BlackPixel(display, DefaultScreen(display));
  attr.border_pixel = BlackPixel(display, DefaultScreen(display));
  mask = CWBackPixel | CWBorderPixel;
  window = XCreateWindow(display, DefaultRootWindow(display),
    0, 0, 320, 192, 1, CopyFromParent, CopyFromParent, CopyFromParent,
    mask, &attr);
  mask = KeyPressMask | KeyReleaseMask;
  XSelectInput(display, window, mask);
  XMapRaised(display, window);
  values.background = BlackPixel(display, DefaultScreen(display));
  values.foreground = WhitePixel(display, DefaultScreen(display));
  values.line_width = 1;
  values.line_style = LineSolid;
  values.cap_style = CapNotLast;
  values.join_style = JoinMiter;
  mask = GCLineWidth | GCForeground | GCBackground | GCCapStyle |
         GCJoinStyle | GCLineStyle;
  gc = XCreateGC(display, window, mask, &values);
  XGetWindowAttributes(display, window, &winattr);
  depth = winattr.depth;
  bytespixel = (depth + 7) / 8;
  bytespixel = 4;
  bytesrow = (320 * bytespixel + bytespixel) /4 * 4;
  data = (char*)malloc(192 * bytesrow);
  image = XCreateImage(display, winattr.visual, depth, ZPixmap, 0, data,
          320, 192, 32, bytesrow);
  XFlush(display);
  tia->ScanLine = 0;
  tia->DotClock = 0;
  tia->colubk = 0;
  tia->grp0 = 0;
  tia->grp1 = 0;
  tia->posp0 = 20;
  tia->posp1 = 132;
  tia->posbl = 140;
  tia->posm0 = 40;
  tia->posm1 = 50;
  tia->hmbl = 0;
  tia->hmbl_i = 0;
  tia->hmp0 = 0;
  tia->hmp0_i = 0;
  tia->hmp1 = 0;
  tia->hmp1_i = 0;
  tia->hmm0 = 0;
  tia->hmm0_i = 0;
  tia->hmm1 = 0;
  tia->hmm1_i = 0;
  tia->cxm0p = 0;
  tia->cxm1p = 0;
  tia->cxp0fb = 0;
  tia->cxp1fb = 0;
  tia->cxm0fb = 0;
  tia->cxm1fb = 0;
  tia->cxblpf = 0;
  tia->cxppmm = 0;
  tia->inpt4 = 0xff;
  tia->inpt5 = 0xff;
  tia->resmp0 = 0;
  tia->resmp1 = 0;
  }

void tia_cycle(TIA *tia) {
  int i;
  int x,y;
  word p;
  word d;
  byte cpf;
  byte cp0;
  byte cp1;
  byte cbl;
  byte cm0;
  byte cm1;
  word ps;
  word pe;
  byte pw;
  byte sh;
  byte cp;
  byte sp;
  KeySym key;
  char tmp[256];
  XComposeStatus status;
  XEvent event;
  if (tia->DotClock == 1 && tia->ScanLine == 232) {
    while (XPending(display) > 0) {
      if (XNextEvent(display, &event) >= 0) {
        if (event.type == KeyPress) {
          XLookupString((XKeyEvent*)&event, tmp, 10, &key, &status);
          if (key == XK_Left) pia.swcha = (pia.swcha & 0xf0) | 0x04; 
          if (key == XK_Right) pia.swcha = (pia.swcha & 0xf0) | 0x08; 
          if (key == XK_Up) pia.swcha = (pia.swcha & 0xf0) | 0x01; 
          if (key == XK_Down) pia.swcha = (pia.swcha & 0xf0) | 0x02; 
          if (key == 'a' || key == 'A') pia.swcha &= 0xbf;
          if (key == 'd' || key == 'D') pia.swcha &= 0x7f;
          if (key == 'w' || key == 'W') pia.swcha &= 0xef;
          if (key == 's' || key == 'S') pia.swcha &= 0xdf;
          if (key == XK_F3) pia.swchb ^= 0x08;
          if (key == XK_F4) pia.swchb ^= 0x40;
          if (key == XK_F5) pia.swchb ^= 0x80;
          if (key == XK_F6) pia.swchb &= 0xfd;
          if (key == XK_F7) pia.swchb &= 0xfe;
          if (key == XK_Shift_L) tia->inpt4 &= 0x7f;
          if (key == XK_Shift_R) tia->inpt5 &= 0x7f;
          }
        else if (event.type == KeyRelease) {
          XLookupString((XKeyEvent*)&event, tmp, 10, &key, &status);
          if (key == XK_Left) pia.swcha = (pia.swcha & 0xf0);
          if (key == XK_Right) pia.swcha = (pia.swcha & 0xf0);
          if (key == XK_Up) pia.swcha = (pia.swcha & 0xf0);
          if (key == XK_Down) pia.swcha = (pia.swcha & 0xf0);
          if (key == 'a' || key == 'A') pia.swcha |= 0x40;
          if (key == 'd' || key == 'D') pia.swcha |= 0x80;
          if (key == 'w' || key == 'W') pia.swcha |= 0x01;
          if (key == 's' || key == 'S') pia.swcha |= 0x02;
          if (key == XK_F2) { power = 'N'; run = 0; }
          if (key == XK_Escape) { power = 'N'; run = 0; }
          if (key == XK_F6) pia.swchb |= 0x02;
          if (key == XK_F7) pia.swchb |= 0x01;
          if (key == XK_Shift_L) tia->inpt4 |= 0x80;
          if (key == XK_Shift_R) tia->inpt5 |= 0x80;
          }
        else printf("Unknown X event\n");
        }
      }
    }

  unsigned int clr;
  if (tia->resmp0 & 2) {
    if ((tia->nusiz0 & 0x07) == 0x05) tia->posm0 = tia->posp0 + 7;
    else if ((tia->nusiz0 & 0x07) == 0x07) tia->posm0 = tia->posp0 + 15;
    else tia->posm0 = tia->posp0 + 3;
    }
  if (tia->resmp1 & 2) {
    if ((tia->nusiz1 & 0x07) == 0x05) tia->posm1 = tia->posp1 + 7;
    else if ((tia->nusiz1 & 0x07) == 0x07) tia->posm1 = tia->posp1 + 15;
    else tia->posm1 = tia->posp1 + 3;
    }
  tia->DotClock++;
  if (tia->DotClock == 228) {
    tia->DotClock = 0;
    tia->ScanLine++;
    cpu.idle = 0;
    if (tia->ScanLine == 260) {
      XPutImage(display, window, gc, image, 0, 0, 0, 0, 320, 192);
      XFlush(display);
      usleep(10000);
      }
    if (tia->ScanLine >= 263) {
      tia->ScanLine = 0;
      }
    }
  if (tia->ScanLine >= 40 && tia->ScanLine < 232 &&
      tia->DotClock >= 68) {
    p = tia->DotClock - 68;
    d = tia->DotClock;
    x = p * 2;
    y = tia->ScanLine - 40;
    if ((tia->vblank & 2) == 0) {
      cpf = 0x00;
      cp0 = 0x00;
      cp1 = 0x00;
      cbl = 0x00;
      cm0 = 0x00;
      cm1 = 0x00;
      if (p < 80) {
        if (tia->playfield & (0x80000 >> (p >> 2))) clr = cpf = 0xff;
        }
      else {
        if ((tia->ctrlpf & 1) != 0) {
          if (tia->playfield & (0x00001 << ((p-80) >> 2))) cpf = 0xff;
          }
        else {
          if (tia->playfield & (0x80000 >> ((p-80) >> 2))) cpf = 0xff;
          }
        }
      if (tia->grp0 != 0) {
        pw = 1; sh = 0; cp = 1; sp = 8;
        if ((tia->nusiz0 & 0x7) == 0x1) { cp = 2; sp = 16; }
        if ((tia->nusiz0 & 0x7) == 0x2) { cp = 2; sp = 32; }
        if ((tia->nusiz0 & 0x7) == 0x3) { cp = 3; sp = 16; }
        if ((tia->nusiz0 & 0x7) == 0x4) { cp = 2; sp = 64; }
        if ((tia->nusiz0 & 0x7) == 0x5) { pw = 2; sh = 1; }
        if ((tia->nusiz0 & 0x7) == 0x6) { cp = 3; sp = 32; }
        if ((tia->nusiz0 & 0x7) == 0x7) { pw = 4; sh = 2; }
        for (i=0; i<cp; i++) {
          ps = i * sp + tia->posp0 + 1;
          if (ps > 228) ps -= 160;
          pe = ps + (8 * pw) - 1;
          if (pe > 228) pe -= 160;
          if ((pe >= ps && d >= ps && d <= pe) ||
              (pe < ps && (d >= ps || d <= pe))) {
            if ((tia->refp0 & 0x08) == 0) {
              if (tia->grp0 & (0x80 >> ((d-ps) >> sh))) cp0 = 0xff;
              }
            else {
              if (tia->grp0 & (0x01 << ((d-ps) >> sh))) cp0 = 0xff;
              }
            }
          }
        }
      if (tia->grp1 != 0) {
        pw = 1; sh = 0; cp = 1; sp = 8;
        if ((tia->nusiz1 & 0x7) == 0x1) { cp = 2; sp = 16; }
        if ((tia->nusiz1 & 0x7) == 0x2) { cp = 2; sp = 32; }
        if ((tia->nusiz1 & 0x7) == 0x3) { cp = 3; sp = 16; }
        if ((tia->nusiz1 & 0x7) == 0x4) { cp = 2; sp = 64; }
        if ((tia->nusiz1 & 0x7) == 0x5) { pw = 2; sh = 1; }
        if ((tia->nusiz1 & 0x7) == 0x6) { cp = 3; sp = 32; }
        if ((tia->nusiz1 & 0x7) == 0x7) { pw = 4; sh = 2; }
        for (i=0; i<cp; i++) {
          ps = i * sp + tia->posp1 + 1;
          if (ps > 228) ps -= 160;
          pe = ps + (8 * pw) - 1;
          if (pe > 228) pe -= 160;
          if ((pe >= ps && d >= ps && d <= pe) ||
              (pe < ps && (d >= ps || d <= pe))) {
            if ((tia->refp1 & 0x08) == 0) {
              if (tia->grp1 & (0x80 >> ((d-ps) >> sh))) cp1 = 0xff;
              }
            else {
              if (tia->grp1 & (0x01 << ((d-ps) >> sh))) cp1 = 0xff;
              }
            }
          }
        }
      if ((tia->enabl & 2) != 0) {
        ps = tia->posbl;
        if (ps > 228) ps -= 160;
        switch (tia->ctrlpf & 0x30) {
          case 0x00: pe = ps; break;
          case 0x10: pe = ps+1; break;
          case 0x20: pe = ps+3; break;
          case 0x30: pe = ps+7; break;
          }
        if (pe > 228) pe -= 160;
        if ((pe >= ps && d >= ps && d <= pe) ||
            (pe < ps && (d >= ps || d <= pe))) {
          cbl = 0xff;
          }
        }
      if ((tia->enam0 & 2) != 0) {
        pw = 1; sh = 0; cp = 1; sp = 8;
        if ((tia->nusiz0 & 0x7) == 0x1) { cp = 2; sp = 16; }
        if ((tia->nusiz0 & 0x7) == 0x2) { cp = 2; sp = 32; }
        if ((tia->nusiz0 & 0x7) == 0x3) { cp = 3; sp = 16; }
        if ((tia->nusiz0 & 0x7) == 0x4) { cp = 2; sp = 64; }
        if ((tia->nusiz0 & 0x7) == 0x6) { cp = 3; sp = 32; }
        for (i=0; i<cp; i++) {
          ps = i*sp + tia->posm0;
          if (ps > 228) ps -= 160;
          switch (tia->nusiz0 & 0x30) {
            case 0x00: pe = ps; break;
            case 0x10: pe = ps+1; break;
            case 0x20: pe = ps+3; break;
            case 0x30: pe = ps+7; break;
            }
          if (pe > 228) pe -= 160;
          if ((pe >= ps && d >= ps && d <= pe) ||
              (pe < ps && (d >= ps || d <= pe))) {
            cm0 = 0xff;
            }
          }
        }
      if ((tia->enam1 & 2) != 0) {
        pw = 1; sh = 0; cp = 1; sp = 8;
        if ((tia->nusiz1 & 0x7) == 0x1) { cp = 2; sp = 16; }
        if ((tia->nusiz1 & 0x7) == 0x2) { cp = 2; sp = 32; }
        if ((tia->nusiz1 & 0x7) == 0x3) { cp = 3; sp = 16; }
        if ((tia->nusiz1 & 0x7) == 0x4) { cp = 2; sp = 64; }
        if ((tia->nusiz1 & 0x7) == 0x6) { cp = 3; sp = 32; }
        for (i=0; i<cp; i++) {
          ps = i * sp + tia->posm1;
          if (ps > 228) ps -= 160;
          switch (tia->nusiz1 & 0x30) {
            case 0x00: pe = ps; break;
            case 0x10: pe = ps+1; break;
            case 0x20: pe = ps+3; break;
            case 0x30: pe = ps+7; break;
            }
          if (pe > 228) pe -= 160;
          if ((pe >= ps && d >= ps && d <= pe) ||
              (pe < ps && (d >= ps || d <= pe))) {
            cm1 = 0xff;
            }
          }
        }
      if (cm0 && cp1) tia->cxm0p |= 0x80;
      if (cm0 && cp0) tia->cxm0p |= 0x40;
      if (cm1 && cp0) tia->cxm1p |= 0x80;
      if (cm1 && cp1) tia->cxm1p |= 0x40;
      if (cp0 && cpf) tia->cxp0fb |= 0x80;
      if (cp0 && cbl) tia->cxp0fb |= 0x40;
      if (cp1 && cpf) tia->cxp1fb |= 0x80;
      if (cp1 && cbl) tia->cxp1fb |= 0x40;
      if (cm0 && cpf) tia->cxm0fb |= 0x80;
      if (cm0 && cbl) tia->cxm0fb |= 0x40;
      if (cm1 && cpf) tia->cxm1fb |= 0x80;
      if (cm1 && cbl) tia->cxm1fb |= 0x40;
      if (cbl && cpf) tia->cxblpf |= 0x80;
      if (cp0 && cp1) tia->cxppmm |= 0x80;
      if (cm0 && cm1) tia->cxppmm |= 0x40;
      clr = tia_colors[tia->colubk];
      if ((tia->ctrlpf & 0x04) == 0) {
        if (cpf) {
          if ((tia->ctrlpf & 2) == 0)
            clr = tia_colors[tia->colupf];
          else
            if (tia->DotClock < 148)
              clr = tia_colors[tia->colup0];
            else
              clr = tia_colors[tia->colup1];
          }
        if (cbl) clr = tia_colors[tia->colupf];
        if (cp1) clr = tia_colors[tia->colup1];
        if (cm1) clr = tia_colors[tia->colup1];
        if (cp0) clr = tia_colors[tia->colup0];
        if (cm0) clr = tia_colors[tia->colup0];
        }
      else {
        if (cp1) clr = tia_colors[tia->colup1];
        if (cm1) clr = tia_colors[tia->colup1];
        if (cp0) clr = tia_colors[tia->colup0];
        if (cm0) clr = tia_colors[tia->colup0];
        if (cpf) {
          if ((tia->ctrlpf & 2) == 0)
            clr = tia_colors[tia->colupf];
          else
            if (tia->DotClock < 148)
              clr = tia_colors[tia->colup0];
            else
              clr = tia_colors[tia->colup1];
          }
        if (cbl) clr = tia_colors[tia->colupf];
        }
      }
    else {
      clr = 0x000000;
      }
    XPutPixel(image, x, y, clr);
    XPutPixel(image, x+1, y, clr);
    }
  }

void tia_write(TIA *tia, byte port, byte value) {
  word v;
  if (port == 0x00) { 
    tia->vsync = value;
    if (value != 0) {
      tia->ScanLine = 0;
      tia->DotClock = 0;
      }
    }
  if (port == 0x01) tia->vblank = value;
  if (port == 0x02) { cpu.idle = 0xff; cpu.clocks = 0; }
  if (port == 0x04) tia->nusiz0 = value;
  if (port == 0x05) tia->nusiz1 = value;
  if (port == 0x06) tia->colup0 = value >> 1;
  if (port == 0x07) tia->colup1 = value >> 1;
  if (port == 0x08) tia->colupf = value >> 1;
  if (port == 0x09) tia->colubk = value >> 1;
  if (port == 0x0a) tia->ctrlpf = value;
  if (port == 0x0b) tia->refp0 = value;
  if (port == 0x0c) tia->refp1 = value;
  if (port == 0x0d) {
    v = ((value & 0x80) >> 7) |
        ((value & 0x40) >> 5) |
        ((value & 0x20) >> 3) |
        ((value & 0x10) >> 1);
    tia->playfield = (tia->playfield & 0xffff) | (v << 16);
    }
  if (port == 0x0e) {
    tia->playfield = (tia->playfield & 0x0f00ff) | (value << 8);
    }
  if (port == 0x0f) {
    v = ((value & 0x80) >> 7) |
        ((value & 0x40) >> 5) |
        ((value & 0x20) >> 3) |
        ((value & 0x10) >> 1) |
        ((value & 0x08) << 1) |
        ((value & 0x04) << 3) |
        ((value & 0x02) << 5) |
        ((value & 0x01) << 7);
    tia->playfield = (tia->playfield & 0x0fff00) | v;
    }

  if (port == 0x10) tia->posp0 = tia->DotClock + 4;
  if (port == 0x11) tia->posp1 = tia->DotClock + 4;
  if (port == 0x12) tia->posm0 = tia->DotClock + 4;
  if (port == 0x13) tia->posm1 = tia->DotClock + 4;
  if (port == 0x14) tia->posbl = tia->DotClock + 4;

  if (port == 0x1b) tia->grp0 = value;
  if (port == 0x1c) tia->grp1 = value;
  if (port == 0x1d) tia->enam0 = value;
  if (port == 0x1e) tia->enam1 = value;
  if (port == 0x1f) tia->enabl = value;
  if (port == 0x20) tia->hmp0 = tia_convertOffset(value);
  if (port == 0x21) tia->hmp1 = tia_convertOffset(value);
  if (port == 0x22) tia->hmm0 = tia_convertOffset(value);
  if (port == 0x23) tia->hmm1 = tia_convertOffset(value);
  if (port == 0x24) tia->hmbl = tia_convertOffset(value);
  if (port == 0x28) tia->resmp0 = value;
  if (port == 0x29) tia->resmp1 = value;
  if (port == 0x2a) {
    tia->posp0 += tia->hmp0;
    tia->posp1 += tia->hmp1;
    tia->posm0 += tia->hmm0;
    tia->posm1 += tia->hmm1;
    tia->posbl += tia->hmbl;
    while (tia->posp0 < 68) tia->posp0 += 160;
    while (tia->posp0 > 228) tia->posp0 -= 160;
    while (tia->posp1 < 68) tia->posp1 += 160;
    while (tia->posp1 > 228) tia->posp1 -= 160;
    while (tia->posm0 < 68) tia->posm0 += 160;
    while (tia->posm0 > 228) tia->posm0 -= 160;
    while (tia->posm1 < 68) tia->posm1 += 160;
    while (tia->posm1 > 228) tia->posm1 -= 160;
    while (tia->posbl < 68) tia->posbl += 160;
    while (tia->posbl > 228) tia->posbl -= 160;
    }
  if (port == 0x2b) {
    tia->hmbl_i = 0;
    tia->hmp0_i = 0;
    tia->hmp1_i = 0;
    tia->hmm0_i = 0;
    tia->hmm1_i = 0;
    }
  if (port == 0x2c) {
    tia->cxm0p = 0;
    tia->cxm1p = 0;
    tia->cxp0fb = 0;
    tia->cxp1fb = 0;
    tia->cxm0fb = 0;
    tia->cxm1fb = 0;
    tia->cxblpf = 0;
    tia->cxppmm = 0;
    }
  }

byte tia_read(TIA *tia, byte port) {
  if (port == 0x30) return tia->cxm0p;
  if (port == 0x31) return tia->cxm1p;
  if (port == 0x32) return tia->cxp0fb;
  if (port == 0x33) return tia->cxp1fb;
  if (port == 0x34) return tia->cxm0fb;
  if (port == 0x35) return tia->cxm1fb;
  if (port == 0x36) return tia->cxblpf;
  if (port == 0x37) return tia->cxppmm;
  if (port == 0x3c) return tia->inpt4;
  if (port == 0x3d) return tia->inpt5;
  }


