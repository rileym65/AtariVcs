#include "header.h"
#include "pia.h"

void pia_init(PIA *pia) {
  pia->tint = 0;
  pia->timer = 0xff;
  pia->divider = 1024;
  pia->ticks = 0;
  pia->subticks = 0;
  pia->swcha = 0xff;
  pia->swchb = 0xff;
  }

void pia_cycle(PIA *pia) {
  pia->subticks++;
  if (pia->subticks >= 3) {
    if (pia->tint == 0) {
      pia->subticks = 0;
      pia->ticks++;
      if (pia->ticks >= pia->divider) {
        pia->ticks = 0;
        pia->timer--;
        if (pia->timer == 0xff) pia->tint = 0xff;
        }
      }
    else {
      pia->timer--;
      }
    }
  }

byte pia_read(PIA *pia, word address) {
  if (address == 0x280) return pia->swcha;
  if (address == 0x282) return pia->swchb;
  if (address == 0x284) return pia->timer;
  return 0xff;
  }

void pia_write(PIA *pia, word address, byte value) {
  if (address == 0x294) {
    pia->tint = 0;
    pia->timer = value;
    pia->divider = 1;
    pia->ticks = 0;
    pia->subticks = 0;
    }
  if (address == 0x295) {
    pia->tint = 0;
    pia->timer = value;
    pia->divider = 8;
    pia->ticks = 0;
    pia->subticks = 0;
    }
  if (address == 0x296) {
    pia->tint = 0;
    pia->timer = value;
    pia->divider = 64;
    pia->ticks = 0;
    pia->subticks = 0;
    }
  if (address == 0x295) {
    pia->tint = 0;
    pia->timer = value;
    pia->divider = 1024;
    pia->ticks = 0;
    pia->subticks = 0;
    }
  }

