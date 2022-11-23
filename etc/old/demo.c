// main.c - example program for rendering a 3D cube using s0lpadeine game engine
#include "s0lpadeine.h"

// leave one of the following uncommented:
//#define SCREEN_TEST
#define DEMO_3D

int main(void) {
  if(!init_screen(128, 128)) {
    printf("error init_screen()");
    return 1;
  }
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
  refresh();
#elif defined(DEMO_3D)
  if(!init_mesh("etc/cube.mesh")) {
    printf("error init_mesh()");
    return 1;
  }



#endif
  stop();
  return 0;
}
