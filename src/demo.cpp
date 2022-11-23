#include "s0lpadeine.h"
#include <cmath>
using namespace std;

mesh meshCube;
mat4x4 matProj;
mat4x4 matRotZ, matRotX;
float fTheta;

void frame(float frame_time) {
  clear(0);
  frame_time = 0.001f; // FIXME

  // Set up rotation matrices
  fTheta += 1.0f * frame_time;

  // Rotation Z
  matRotZ.m[0][0] = cosf(fTheta);
  matRotZ.m[0][1] = sinf(fTheta);
  matRotZ.m[1][0] = -sinf(fTheta);
  matRotZ.m[1][1] = cosf(fTheta);
  matRotZ.m[2][2] = 1;
  matRotZ.m[3][3] = 1;

  // Rotation X
  matRotX.m[0][0] = 1;
  matRotX.m[1][1] = cosf(fTheta * 0.5f);
  matRotX.m[1][2] = sinf(fTheta * 0.5f);
  matRotX.m[2][1] = -sinf(fTheta * 0.5f);
  matRotX.m[2][2] = cosf(fTheta * 0.5f);
  matRotX.m[3][3] = 1;

  project();
  refresh();
  return;
}

int main() {
  init_screen(128, 128);
  meshCube.tris = {
    // SOUTH
    { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

    // EAST
    { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
    { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

    // NORTH
    { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
    { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

    // WEST
    { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

    // TOP
    { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
    { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

    // BOTTOM
    { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
    { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
  };

  // Projection Matrix
  float fNear = 0.1f;
  float fFar = 1000.0f;
  float fFov = 90.0f;
  float fAspectRatio = (float)get_screen_height() / (float)get_screen_width();
  float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

  matProj.m[0][0] = fAspectRatio * fFovRad;
  matProj.m[1][1] = fFovRad;
  matProj.m[2][2] = fFar / (fFar - fNear);
  matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
  matProj.m[2][3] = 1.0f;
  matProj.m[3][3] = 0.0f;

  for(unsigned i = 0; i < 100000; i++) {
    frame(0.001f);
  }
  stop();
  return 0;
}
