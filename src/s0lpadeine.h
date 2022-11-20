// s0lpadeine - game engine that renders in ascii
// letters are the new pixels
// copyleft (ɔ) huell8

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

bool** screen; // screen contents
unsigned screen_width, screen_height;
// how screen is defined:
//   example screen:
//    /--------\  <- this is a example of a 8 by 6 screen. It's 8x3 ascii characters and since each character in a monospace
// 45 |  '     |     font is x by 2x, it's actually defined to have 8 by 6 cells. To initialize a screen like this one you would use:
// 23 |     .  |             initialize(8, 6);
// 01 | #      |    Each cell of the screen is actually a boolean value and belongs to exactly one pair. Pair are defined to
//    \--------/    contain the cells screen[x][2*n] and screen[x][2*n+1]. Pairs are how the bool values are drawn on screen
//     01234567     each arrangement of bits in a pair corresponds to a ascii character.
//                   - the . in the example screen is drawn by calling: draw_point(5, 2);
//                   - the ' in the example screen is drawn by calling: draw_point(2, 5);
//                   - the # in the example screen is drawn by calling: draw_point(1, 0); and draw_point(1, 1);
//


void initialize(unsigned width, unsigned height) {
  // memory allocation for the screen contents
  screen = (bool**)malloc(width * sizeof(bool));
  for(unsigned i = 0; i < width; i++)
    screen[i] = (bool*)malloc(height * sizeof(bool));
  screen_width = width; screen_height = height;
  // ansi magic
  printf("\e[?25l"); // hide cursor
  printf("\e[2J");   // erase entire screen
  return;
}

void stop() {
  // always call it and the end of your program
  free(screen);
  printf("\e[?25h"); // show cursor
  return;
}

void clear(bool v) {
  for(unsigned x = 0; x < screen_width; x++) {
    for(unsigned y = 0; y < screen_height; y++) {
      printf("no coredump for x: %d and y: %d\n", x, y);
      screen[x][y] = v;
    }
  }
  return;
}
void test() {
  printf("running test:\n");
  printf("width: %d, height: %d\n", screen_width, screen_height);
  screen[screen_width-1][screen_height-1] = 1;
  printf("no coredump 1!\n");
  clear(0);
  printf("no coredump 3!\n");
}

void refresh() {
  // draws contents of screen to the actual screen
  printf("\e[H"); // move cursor to home position
  for(int y = screen_height-2; y >= 0; y-=2) { // y becomes negative when the loop must end
                                               // that's why it's of type int and not unsigned
    for(unsigned x = 0; x < screen_width; x++) {
      // draw rules
      if(screen[x][y]) {
        // bottom cell of the pair is on
        if(screen[x][y+1]) {
          // bottom and top cell of the pair are on
          printf("#");
        } else {
          // bottom cell is on and top cell is off
          printf(".");
        }
      } else {
        // bottom cell of the pair is off
        if(screen[x][y+1]) {
          // bottom cell is off and top cell is on
          printf("'");
        } else {
          // bottom and top cell of the pair are off
          printf(" ");
        }
      }
    }
    printf("\n");
  }
  fflush(stdout); // printf correctly and all the things
  return;
}

void test_animation() {
  while(true) {
    for(unsigned x = 0; x < screen_width; x++)
      for(unsigned y = 0; y < screen_height; y++)
        screen[x][y] = !screen[x][y];
    sleep(0.5);
    refresh();
  }
  return;
}
