// main.c - example program for rendering a 3D cube using s0lpadeine game engine
#include "s0lpadeine.h"

int main(void) {
  initialize(128, 128);
  clear(0);
  // border
  draw_line(0, 0, 0, 127, 1);
  draw_line(0, 0, 127, 0, 1);
  draw_line(0, 127, 127, 127, 1);
  draw_line(127, 0, 127, 127, 1);
  // diagonals
  draw_line(0, 0, 127, 127, 1);
  draw_line(0, 127, 127, 0, 1);

  refresh();
  stop();
  return 0;
}
