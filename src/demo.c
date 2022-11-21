// main.c - example program for rendering a 3D cube using s0lpadeine game engine
#include "s0lpadeine.h"

int main(void) {
  initialize(128, 128);
  clear(0);
  test();
  refresh();
  draw_point(0, 0); 
  draw_point(127, 127);
  refresh(); 
  stop();
  return 0;
}
