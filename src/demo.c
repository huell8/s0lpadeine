// main.c - example program for rendering a 3D cube using s0lpadeine game engine
#include "s0lpadeine.h"

// leave one of the following uncommented:
#define SCREEN_TEST
//#define TRIANGLES_DEMO

int main(void) {
  initialize(128, 128);
  clear(0);
 #if defined(SCREEN_TEST)
  // border
  draw_line(0, 0, 0, 127, 1);
  draw_line(0, 0, 127, 0, 1);
  draw_line(0, 127, 127, 127, 1);
  draw_line(127, 0, 127, 127, 1);
  // diagonals
  draw_line(0, 0, 127, 127, 1);
  draw_line(0, 127, 127, 0, 1);
#elif defined(TRIANGLES_DEMO)
  // triangles demo
  draw_triangle(64, 64, 74, 79, 74, 49, 1);
  draw_triangle(64, 64, 54, 79, 54, 49, 1);
  draw_triangle(64, 64, 59, 0, 68, 0, 1);
#endif

  refresh();
  stop();
  return 0;
}
