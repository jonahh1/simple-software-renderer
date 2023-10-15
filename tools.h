#ifndef TOOLS_H
#define TOOLS_H

#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "string.h"

void LoadGameLevel(const char *levelName);
#include "game.h"

struct Sector
{
  Vector2* points;
  int lenPoints;

  int floorHeight;
  int ceilingHeight;

} typedef Sector;

struct Level
{
  Sector* sectors;
  int lenSectors;
} typedef Level;

/*Level LoadLevelFromTextFile(char* fileName)
{
  // to be implemented
}*/
//void LoadGameLevel(const char *levelName);

void LoadGameLevel(const char *levelName) {
  
  char *levelPathPrefix = "assets/levels/";
  char *levelPath = (char*)malloc(sizeof(levelPathPrefix) + sizeof(levelName) + 4 * sizeof(char));
  strcpy(levelPath, levelPathPrefix);
  strcat(levelPath, levelName);
  strcat(levelPath, ".png");

  Image map = LoadImage(levelPath);
  GameLevel = (int*)malloc(map.width*map.height * sizeof(int));

  for (int y = 0; y < map.height; y++) {
    for (int x = 0; x < map.width; x++) {
      int blockType = 0;
      Color imageCol = ((Color*)map.data)[y*map.width+x];

      //Color ic = *((Color *)imageCol[y*map.width+x]);
      /*if (!ColorEqualsColor(imageCol, (Color){0,0,0,255})) {
        blockType = 1;
      }*/
      if (imageCol.a != 0) {
        blockType = 1;
      }
      GameLevel[y*map.width + x] = blockType;
    }
  }
  LevelWidth = map.width;
  LevelHeight = map.height;
  //free(levelPathPrefix);
  //free(levelPath);
  UnloadImage(map);
}
#endif