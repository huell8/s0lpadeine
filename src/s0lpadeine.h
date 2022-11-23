// s0lpadeine - game engine that renders in ascii
// letters are the new pixels
// copyleft (ɔ) huell8

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <cmath>

struct vec3d{
  float x, y, z;
};

struct triangle{
  vec3d p[3];
};

struct mesh {
  std::vector<triangle> tris;
};

struct mat4x4 {
  float m[4][4] = { 0 };
};

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

mesh meshCube;
mat4x4 projection_mat;
extern mat4x4 matRotZ, matRotX;

bool init_screen(unsigned width, unsigned height) {
  // memory allocation for the screen contents
  screen = (bool**)malloc(width * sizeof(bool*));
  for(unsigned i = 0; i < width; i++)
    screen[i] = (bool*)malloc(height * sizeof(bool));
  screen_width = width; screen_height = height;
  // ansi magic
  printf("\e[?25l"); // hide cursor
  printf("\e[2J");   // erase entire screen
  return true;
}

bool init_projection_mat(float near, float far, float fov) {
  float fAspectRatio = (float)screen_height / (float)screen_width;
  float fovRad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);

  projection_mat.m[0][0] = fAspectRatio * fovRad;
  projection_mat.m[1][1] = fovRad;
  projection_mat.m[2][2] = far / (far - near);
  projection_mat.m[3][2] = (-far * near) / (far - near);
  projection_mat.m[2][3] = 1.0f;
  projection_mat.m[3][3] = 0.0f;
  return true;
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
    draw_point(lerpx, lerpy, v);
  }
}

void draw_triangle(unsigned x0, unsigned y0, unsigned x1, unsigned y1, unsigned x2, unsigned y2, bool v) {
	// just call draw_line() 3 times 
	draw_line(x0, y0, x1, y1, v);
	draw_line(x1, y1, x2, y2, v);
	draw_line(x2, y2, x0, y0, v);
	return;
}

void multiply_matrix_vector(vec3d &i, vec3d &o, mat4x4 &m) {
  o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
  o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
  o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
  float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];
  if (w != 0.0f) {
    o.x /= w; o.y /= w; o.z /= w;
  }
}

void project() {
  for (auto tri : meshCube.tris) {
    triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;
      // Rotate in Z-Axis
      multiply_matrix_vector(tri.p[0], triRotatedZ.p[0], matRotZ);
      multiply_matrix_vector(tri.p[1], triRotatedZ.p[1], matRotZ);
      multiply_matrix_vector(tri.p[2], triRotatedZ.p[2], matRotZ);

      // Rotate in X-Axis
      multiply_matrix_vector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
      multiply_matrix_vector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
      multiply_matrix_vector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

      // Offset into the screen
      triTranslated = triRotatedZX;
      triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
      triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
      triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

      // Project triangles from 3D --> 2D
      multiply_matrix_vector(triTranslated.p[0], triProjected.p[0], projection_mat);
      multiply_matrix_vector(triTranslated.p[1], triProjected.p[1], projection_mat);
      multiply_matrix_vector(triTranslated.p[2], triProjected.p[2], projection_mat);

      // Scale into view
      triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
      triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
      triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
      triProjected.p[0].x *= 0.5f * (float)screen_width;
      triProjected.p[0].y *= 0.5f * (float)screen_height;
      triProjected.p[1].x *= 0.5f * (float)screen_width;
      triProjected.p[1].y *= 0.5f * (float)screen_height;
      triProjected.p[2].x *= 0.5f * (float)screen_width;
      triProjected.p[2].y *= 0.5f * (float)screen_height;

      // Rasterize triangle
      draw_triangle(triProjected.p[0].x, triProjected.p[0].y,
                    triProjected.p[1].x, triProjected.p[1].y,
                    triProjected.p[2].x, triProjected.p[2].y,
                    1);
  }
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
