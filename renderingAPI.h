#ifndef RENDERING_API_H
#define RENDERING_API_H

#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "raylib.h"

#include "softwareAPI.h"
#include "game.h"
struct HitInfo {
  float distance;
  int blockTypeHit;
} typedef HitInfo;


HitInfo Raycast(float x);
void DrawColumns();
void DrawObjects();

float sampleX = 0;
HitInfo Raycast(float x) {
  float rayAngle = (PlayerDirection - PlayerFOV / 2) + ((float)x / ScreenBuffer.width) * PlayerFOV;
  HitInfo info = {0,0};
  bool hitWall = false;
  float maxDistance = LevelWidth;

  float eyeX = cos(rayAngle);
  float eyeY = sin(rayAngle);

  while (info.distance < maxDistance && !hitWall) {
    info.distance += 0.05;

    float testX = PlayerPostion.x + (eyeX * info.distance);
    float testY = PlayerPostion.y + (eyeY * info.distance);
    int ITestX = (int)testX;
    int ITestY = (int)testY;

    if (ITestX < 0 || ITestX > maxDistance || ITestY < 0 || ITestY > maxDistance)
    {
      hitWall = 1;
      info.distance = maxDistance;
      info.blockTypeHit = 1;
    }
    else
    {
      info.blockTypeHit = GameLevel[(int)ITestY * LevelWidth + (int)ITestX];

      float blockMidX = ITestX+.5;
      float blockMidY = ITestY+.5;
      if (info.blockTypeHit == 3) {
        float vecX = blockMidX - testX;
        float vecY = blockMidY - testY;
        float dist = sqrt(vecX*vecX + vecY*vecY);
        if (dist < 0.5) {
          hitWall = 1;
        }
      }
      else if (info.blockTypeHit != 0) {
        hitWall = 1;

        float testAngle = atan2f((testY-blockMidY), (testX-blockMidX));
        
        if (testAngle >= -PI * 0.25f && testAngle <   PI * 0.25f) sampleX = testY - (float)ITestY;
				if (testAngle >=  PI * 0.25f && testAngle <   PI * 0.75f) sampleX = testX - (float)ITestX;
				if (testAngle <  -PI * 0.25f && testAngle >= -PI * 0.75f) sampleX = testX - (float)ITestX;
				if (testAngle >=  PI * 0.75f || testAngle <  -PI * 0.75f) sampleX = testY - (float)ITestY;

        //return hitValue;
      }

      /*if (info.blockTypeHit != 0) {
        
        DrawLine(PlayerPostion.x*MinimapScale, PlayerPostion.y*MinimapScale, testX*y, testY*mapScale, (Color){255,255,255,128});
        //DrawPixel(testX*mapScale, testY*mapScale, RED);
      }*/
    }
  }
  return info;
}
void DrawColumns() {
    SetRandomSeed(0);
  for (int x=0; x < ScreenBuffer.width; x++) {
    HitInfo info = Raycast(x);
    info.distance = info.distance/(LevelWidth);
    Color color = {info.distance*255,info.distance*255,info.distance*255,255};
    DrawColumn(x, (1-info.distance)*ScreenBuffer.height/2, ScreenBuffer.height-((1-info.distance)*ScreenBuffer.height/2), color);
  }
}

#endif