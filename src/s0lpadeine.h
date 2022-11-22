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
  screen = (bool**)malloc(width * sizeof(bool*));
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
  for(unsigned x = 0; x < screen_width; x++)
    for(unsigned y = 0; y < screen_height; y++)
      screen[x][y] = v;
  return;
}

void draw_point(unsigned x, unsigned y, bool v) {
  // set pint (x, y) to v
  if(x < screen_width && y < screen_height)
    screen[x][y] = v;
  return;
}
void draw_point_fast(unsigned x, unsigned y, bool v) {
  // set point (x, y) to v
  // use only if you are * s u p e r * sure x and y wont be to big
  screen[x][y] = v;
  return;
}

void draw_line(unsigned x0, unsigned y0, unsigned x1, unsigned y1, bool v) {
// the idea is to divide the line into smaller segments of length equal to one cells' size
  // and then for each point draw its aproximation
  //
  // make the line fit on the screen:
  if(x0 >= screen_width)  x0 = screen_width  - 1;
  if(x1 >= screen_width)  x1 = screen_width  - 1;
  if(y0 >= screen_height) y0 = screen_height - 1;
  if(y1 >= screen_height) y1 = screen_height - 1;
  if(x0 == x1 && y0 == y1) draw_point(x0, y0, v); //so we dont divide by 0
  //       distance = max(abs(x0-x1), abs(y0-y1)) - number of smaller segments minus one
  unsigned distance = (x0>x1?x0-x1:x1-x0)>(y0>y1?y0-y1:y1-y0)?(x0>x1?x0-x1:x1-x0):(y0>y1?y0-y1:y1-y0);
  for(unsigned step = 0; step <= distance; step++) {
    float t = (float)step/(float)distance;
    // draw_point_fast(round(lerp(x0, x1, t)), round(lerp(y0, y1, t)), v)
    // calculate interpolated values:
    //                           standard interplation            adding 0.5f to round correctly
    unsigned lerpx = (unsigned)((float)x0 * (1.0f - t) + t * (float)x1 + 0.5f);
    unsigned lerpy = (unsigned)((float)y0 * (1.0f - t) + t * (float)y1 + 0.5f);
    draw_point_fast(lerpx, lerpy, v);
  }
}

void draw_triangle(unsigned x0, unsigned y0, unsigned x1, unsigned y1, unsigned x2, unsigned y2, bool v) {
	// just call draw_line() 3 times 
	draw_line(x0, y0, x1, y1, v);
	draw_line(x1, y1, x2, y2, v);
	draw_line(x2, y2, x0, y0, v);
	return;
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
