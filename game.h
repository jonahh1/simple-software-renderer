#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "string.h"

#include "softwareAPI.h"
#include "utils.h"

Vector2 PlayerPostion = (Vector2){5.5,5.5};
Vector2 playerDelta = {.1,.1};
float PlayerDirection = 0;
float PlayerFOV = PI * 1/3;

int *GameLevel;
int LevelWidth = 0;
int LevelHeight = 0;
float MinimapScale = 5;

void DrawMinimap() {
  //printf("what\n");
  for (int y = 0; y < LevelHeight; y++) {
    for (int x = 0; x < LevelWidth; x++) {
      int g = 1-GameLevel[y*LevelWidth+x];
      DrawRect(
        x*MinimapScale,y*MinimapScale, MinimapScale,MinimapScale,
        (Color){g*255,g*255,g*255,255}
      );
    }
  }
  //if (roundf(PlayerPostion.x*MinimapScale) == x*MinimapScale && roundf(PlayerPostion.y*MinimapScale) == y*MinimapScale)
  DrawRect(roundf(PlayerPostion.x*MinimapScale - 1.5), roundf(PlayerPostion.y*MinimapScale - 1.5), 3, 3, GREEN);
  
  float dx = (float)cos(PlayerDirection);
  float dy = (float)sin(PlayerDirection);

  InvertPixel((PlayerPostion.x+dx)*MinimapScale, (PlayerPostion.y+dy)*MinimapScale);
}
void ControlPlayer() {
  Vector2 playerPosBeforeMoving = PlayerPostion;
  // moving forwards and backwards
  if (IsKeyDown(KEY_W))
  {
    PlayerPostion.x += playerDelta.x;
    PlayerPostion.y += playerDelta.y;

  }
  if (IsKeyDown(KEY_S))
  {
    PlayerPostion.x -= playerDelta.x;
    PlayerPostion.y -= playerDelta.y;
    
  }
  // strafing left and right
  if (IsKeyDown(KEY_A))
  {
    PlayerPostion.x -= -playerDelta.y;
    PlayerPostion.y -= playerDelta.x;
  }
  if (IsKeyDown(KEY_D))
  {
    PlayerPostion.x -= playerDelta.y;
    PlayerPostion.y -= -playerDelta.x;
  }
  // turing left and right
  if (IsKeyDown(KEY_RIGHT))
  {
    PlayerDirection += 0.1f;
  }
  if (IsKeyDown(KEY_LEFT))
  {
    PlayerDirection -= 0.1f;
  }
  float angleDelta = GetMouseDelta().x/(float)GetScreenWidth();
  angleDelta *= 2;
  PlayerDirection += angleDelta;
  if (angleDelta < 0)
    if (PlayerDirection < 0)
      PlayerDirection = 2 * PI;

  if (angleDelta > 0)
    if (PlayerDirection > 2 * PI)
      PlayerDirection = 0;


  float speed = 0.1;
  playerDelta.x = cos(PlayerDirection) * speed;
  playerDelta.y = sin(PlayerDirection) * speed;

  // int ibm = (int)playerPosBeforeMoving.y * LevelWidth + (int)playerPosBeforeMoving.x; // i before moving
  int iam = (int)PlayerPostion.y * LevelWidth + (int)PlayerPostion.x; // i after moving
 
  if (GameLevel[iam] != 0) {
    /*if (abs(iam-ibm) < LevelWidth) PlayerPostion.x = playerPosBeforeMoving.x;
    else if (abs(iam-ibm) == LevelWidth) PlayerPostion.y = playerPosBeforeMoving.y;*/
    PlayerPostion = playerPosBeforeMoving;
    
    //if (abs(iam-ibm) >= LevelWidth) PlayerPostion.y = playerPosBeforeMoving.y;
    //if (abs(iam-ibm) < LevelWidth) PlayerPostion.x = playerPosBeforeMoving.x;

  }
  PlayerFOV += GetMouseWheelMove()/10;
}
void DrawWorld();
#endif