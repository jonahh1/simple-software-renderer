#ifndef SOFTWARE_API_H
#define SOFTWARE_API_H

#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

void SetPixel(float x, float y, Color color);
void DrawRect(float x, float y, float width, float height, Color color);
void InitScreenBuffer(int width, int height);
void DeInitScreenBuffer();
void UpdateScreenBuffer();
void DrawScreenBuffer();
bool ColorEqualsColor(Color a, Color b);

Texture2D ScreenBuffer;
Color *Pixels;
int ScreenBufferPixelSize = 2;

void SetPixel(float x, float y, Color color) {
  Pixels[(int)(((int)y*ScreenBuffer.width) + (int)x)] = color;
}
void InvertPixel(float x, float y) {
  int i =(int)(((int)y*ScreenBuffer.width) + (int)x);
  Color color = Pixels[i];
  color.r = 255-color.r;
  color.g = 255-color.g;
  color.b = 255-color.b;
  Pixels[i] = color;
}
void DrawRect(float x, float y, float width, float height, Color color) {
  for (int Y=0; Y<height; Y++) {
    for (int X=0; X<width; X++) {
      Pixels[(int)((y+Y)*ScreenBuffer.width + x+X)] = color;
    }
  }
  //
}
void DrawColumn(float x, float y1, float y2, Color color) {
  for (int y=y1; y<y2; y++) {
    Pixels[(int)(y*ScreenBuffer.width + (int)x)] = color;
  }
}

bool textureHasBeenLoaded = false;
void InitScreenBuffer(int width, int height) {
  width /= ScreenBufferPixelSize;
  height /= ScreenBufferPixelSize;
  Image image = GenImageColor(width, height, BLUE);
  ScreenBuffer = LoadTextureFromImage(image);
  UnloadImage(image);
  if (Pixels != NULL) Pixels = (Color*)malloc(height * width * sizeof(Color));
  else Pixels = (Color*)malloc(height * width * sizeof(Color));
  for (int i = 0; i < width*height; i++) {
    Pixels[i] = BLUE;
  }
}
void DeInitScreenBuffer() {
  free(Pixels);
  UnloadTexture(ScreenBuffer);
  textureHasBeenLoaded = true;
}

void UpdateScreenBuffer() {
  if (IsWindowResized()) {
    InitScreenBuffer(GetScreenWidth(), GetScreenWidth());
  }

  UpdateTexture(ScreenBuffer, Pixels);
}

void DrawScreenBuffer() {
  DrawTextureEx(ScreenBuffer, (Vector2){0,0}, 0, ScreenBufferPixelSize, WHITE);
}

bool ColorEqualsColor(Color a, Color b) {
  return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

#endif