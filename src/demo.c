// main.c - example program for rendering a 3D cube using s0lpadeine game engine
#include "s0lpadeine.h"

int main(void) {
  initialize(128, 128);
  clear(0);
  draw_point(0, 0, 1);
  draw_point(127, 127, 1);
  draw_line(10, 10, 117, 117, 1);
  draw_line(100, 34, 34, 100, 1);
  draw_line(30, 30, 80, 42, 1);
  refresh(); 
  stop();
  return 0;
}
