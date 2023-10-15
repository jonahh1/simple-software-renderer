#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"
#include "rlgl.h"
#include <math.h>

static void UpdateDrawFrame(void);
static float rayCast(float rayAngle, float* d, int checkD1);
const int screenWidth = 800;
const int screenHeight = 450;
int bufferWidth = screenWidth;
int bufferHeight = screenHeight;

Texture2D screenBuffer;
Color *pixels;
Vector2 playerPos = {1.5, 1.5};
Vector2 playerDelta = {1, 1};
float playerAngle = 0;
float fov = PI * .5;

float mapScale = 10;

const int mapSize = 20;
Color colorPallet[] = {
  (Color){0,0,0,0},
  (Color){29,  43,  83,  255},
  (Color){126, 37,  83,  255},
  (Color){0,   135, 81,  255},
  (Color){171, 82,  54,  255},
  (Color){95,  87,  79,  255},
  (Color){194, 195, 199, 255},
  (Color){255, 241, 232, 255},
  (Color){255, 0,   77,  255},
  (Color){255, 163, 0,   255},
  (Color){255, 236, 39,  255},
  (Color){0,   228, 54,  255},
  (Color){41,  173, 255, 255},
  (Color){131, 118, 156, 255},
  (Color){255, 119, 168, 255},
  (Color){255, 204, 170, 255},
};
int map[] = {
1,2,1,1,2,1,1,2,3,3,2,1,1,2,1,1,2,1,1,1,
2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,
1,0,0,0,0,3,0,0,0,0,0,3,0,0,0,0,3,1,0,2,
2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,1,0,1,
1,0,0,0,0,3,0,0,0,0,3,0,0,0,0,0,3,1,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,2,
2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,2,
2,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1,0,1,
1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,1,
0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,2,
2,0,0,1,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,1,
1,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,1,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,2,
2,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,
1,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,2,
1,1,1,2,1,1,2,1,1,2,1,1,2,1,1,2,1,1,2,1,
};

int texture1[] = {
  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,
  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,
  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,
  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,

  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,
  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,
  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,
  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,

  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,
  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,
  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,
  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,

  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,
  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,
  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,
  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,
};
int texture2[] = {
  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,
  7, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 7,
  6, 12,12,12, 12,12,12,12, 12,12,12,12, 12,12,12,6,
  6, 12,12,12, 12,12,12,12, 12,12,12,12, 12,12,12,6,

  6, 12,12,12, 12,12,12,12, 12,12,12,12, 12,12,12,6,
  6, 12,12,12, 12,12,12,12, 12,12,12,12, 12,12,12,6,
  6, 12,12,12, 12,12,12,12, 12,12,12,12, 12,12,12,6,
  7, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 7,

  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,
  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,
  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,
  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,

  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,
  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,
  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,
  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,
};
int texture3[] = {
  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,
  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,
  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,
  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,

  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,
  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,  7, 7, 7, 7,
  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,
  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,

  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,
  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,
  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,
  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,

  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,  6, 6, 6, 6,
  13,13,13,13, 13,13,13,13, 13,13,13,13, 13,13,13,13,
  13,13,13,13, 13,13,13,13, 13,13,13,13, 13,13,13,13,
  13,13,13,13, 13,13,13,13, 13,13,13,13, 13,13,13,13,
};
int billboardSprite[] = {
  0, 0, 0,  0, 0, 0,  0,  0, 0, 0,  0, 0, 0,
  0, 0, 0,  0, 0, 0,  0,  0, 0, 0,  0, 0, 0,
  0, 0, 0,  0, 0,13, 13, 13, 0, 0,  0, 0, 0,
  0, 0, 0,  0, 0,13, 13, 13, 0, 0,  0, 0, 0,

  0, 0, 0,  0,13,13, 13, 13,13, 0,  0, 0, 0,
  0, 0, 0,  0,13,13, 13, 13,13, 0,  0, 0, 0,
  0, 0, 0,  0, 0, 0,  6,  0, 0, 0,  0, 0, 0,
  0, 0, 0,  0, 0, 0,  6,  0, 0, 0,  0, 0, 0,

  0, 0, 0,  0, 0, 6,  6,  6, 0, 0,  0, 0, 0,
  4, 4, 4,  4, 4, 4,  4,  4, 4, 4,  4, 4, 4,
  0, 0, 0,  4, 4, 4,  4,  4, 4, 4,  0, 0, 0,
  0, 0, 0,  0, 0, 4,  4,  4, 0, 0,  0, 0, 0,

  0, 0, 0,  0, 0, 4,  4,  4, 0, 0,  0, 0, 0,
  0, 0, 0,  0, 0, 4,  4,  4, 0, 0,  0, 0, 0,
  0, 0, 0,  0, 0, 4,  4,  4, 0, 0,  0, 0, 0,
  0, 0, 0,  4, 4, 4,  4,  4, 4, 4,  0, 0, 0,
};
const float billboardSpriteAspectRatio = 16.0/13.0;

struct sprite
{
  Vector2 position;
  float aspectRatio;
  int* spritePtr[16*13];
} typedef sprite;

sprite sprites[3];

int CompareVector2X(const void *a, const void *b) {
  Vector2 vectorA = *(const Vector2 *)a;
  Vector2 vectorB = *(const Vector2 *)b;
  
  if (vectorA.x < vectorB.x) return -1;
  if (vectorA.x > vectorB.x) return 1;
  return 0;
}

void SortVector2ArrayByX(Vector2 *vectors, int numVectors) {
  qsort(vectors, numVectors, sizeof(Vector2), CompareVector2X);
}

float sampleX = 0;
static void UpdateBuffer(void)
{

  /*for (int y = 0; y < bufferHeight; y++)
   {
     for (int x = 0; x < bufferWidth; x++)
     {
       if ((x + y) % 2 == 0) pixels[y * bufferWidth + x] = GREEN;
       else pixels[y * bufferWidth + x] = WHITE;
     }
  }*/
  for (int x = 0; x < bufferWidth; x++)
  {
    float rayAngle = (playerAngle - fov / 2) + ((float)x / bufferWidth) * fov;

    float d = 0;

    int hitType = rayCast(rayAngle, &d, 1);

    float colorD = (float)1-d / (float)(mapSize);

    Color color;

    for (int y = 0; y < bufferHeight; y++)
    {
      
      int roof = (int)((float)(bufferHeight/2) - bufferHeight/d);
      int floor = bufferHeight-roof;
      
      if (y > roof && y < floor) {
        // draw room
        color = colorPallet[hitType];
        //color = (Color){hitType*15,hitType*15,hitType*15};
        /*int ISampleY = ((float)y/bufferHeight)*16;
        int ISampleX = sampleX*16;
        color = colorPallet[texture1[ISampleY * 16 + ISampleX]];*/
        float sampleY = ((float)y - (float)roof) / ((float)floor - (float)roof);
        int ISampleX = fmod(sampleX*16,16);
        int ISampleY = fmod(sampleY*16,16);


        if      (hitType == 3) color = colorPallet[texture1[ISampleY * 16 + ISampleX]];
        else if (hitType == 1) color = colorPallet[texture1[ISampleY * 16 + ISampleX]];
        else if (hitType > 0)  color = colorPallet[texture2[ISampleY * 16 + ISampleX]];

        color = (Color){(int)((float)color.r*colorD), (int)((float)color.g*colorD), (int)((float)color.b*colorD), 255};
        //free(texToS);
      }
      else if (y >= floor) {
        color = colorPallet[13];
        float floorD = ((float)y/(float)bufferHeight)/2;
        color = (Color){(int)((float)color.r*floorD), (int)((float)color.g*floorD), (int)((float)color.b*floorD), 255};
      }
      else {
        color = colorPallet[7];
        float roofD = 1.-((float)y/(float)bufferHeight);
        color = (Color){(int)((float)color.r*roofD), (int)((float)color.g*roofD), (int)((float)color.b*roofD), 255};
      }
      if (hitType > 0) {
        pixels[y * bufferWidth + x] = color;
      }
      /*else if (hitType < 0 && y > bufferHeight/2) {
        pixels[y * bufferWidth + x] = color;
      }*/
    }
  }
  // draw sprites
  for (int i = 0; i < 3; i++)
  {
    float vecX = sprites[i].position.x - playerPos.x;
    float vecY = sprites[i].position.y - playerPos.y;

    float distanceFromPlayer = sqrtf(vecX*vecX + vecY*vecY);
    float spriteAngle = atan2f(sin(-playerAngle), cos(-playerAngle)) - atan2f(vecY, vecX);
    if (spriteAngle < -PI) spriteAngle += 2*PI;
    if (spriteAngle > PI) spriteAngle -= 2*PI;

    int inPlayerFOV = fabs(spriteAngle) < fov / 2.;

    if (inPlayerFOV && distanceFromPlayer > 2  && distanceFromPlayer < 20) {
      float spriteRoof = (float)(bufferHeight/2.0) - bufferHeight / distanceFromPlayer;
      float spriteFloor = bufferHeight - spriteRoof;
      float spriteHeight = spriteFloor - spriteRoof;

      float spriteWidth = spriteHeight / sprites[i].aspectRatio;

      float middleSprite = (0.5f * (spriteAngle / (fov / 2.0f)) + 0.5f) * (float)bufferHeight;
      
      
      // drawing sprite
      //if (playerAngle-spriteAngle < PI/10)
      for (int x=0; x < spriteWidth; x++) {
        for (int y=0; y < spriteHeight; y++) {
          int sampleX = (int)((x/spriteWidth)*13);
          int sampleY = (int)((y/spriteHeight)*16);

          int* tex[16*13] = {*sprites[i].spritePtr};
         Color color = colorPallet[(*tex)[sampleY * 13 + sampleX]];

          int spriteColumn = (int)(middleSprite+sampleX-(spriteWidth/2.0));

          if (spriteColumn >= 0 && spriteColumn < bufferHeight)
						if (color.a != 0 && y < bufferHeight/2)	{
              pixels[(int)(spriteRoof+sampleY) * bufferWidth + spriteColumn] = color;
							//Draw(nObjectColumn, fObjectCeiling + ly, c, object.sprite->SampleColour(fSampleX, fSampleY));
							//fDepthBuffer[nObjectColumn] = fDistanceFromPlayer;
						}
        }
      }

      //pixels[y * bufferWidth + x] = BLACK;

    }
  }
  UpdateTexture(screenBuffer, pixels);
}
static float rayCast(float rayAngle, float* d, int checkD1) {
  float hitValue = 0;
  float eyeX = cos(rayAngle);
  float eyeY = sin(rayAngle);
  int hitWall = 0;
  int corner = 0;
  while (!hitWall && (*d) < mapSize)
  {
    (*d) += 0.05;
    float testX = playerPos.x + (eyeX * (*d));
    float testY = playerPos.y + (eyeY * (*d));
    int ITestX = (int)(playerPos.x + (eyeX * (*d)));
    int ITestY = (int)(playerPos.y + (eyeY * (*d)));

    if (ITestX < 0 || ITestX > mapSize || ITestY < 0 || ITestY > mapSize)
    {
      hitWall = 1;
      (*d) = mapSize;
      hitValue = 1;
    }
    else
    {
      hitValue = map[(int)ITestY * mapSize + (int)ITestX];

      float blockMidX = ITestX+.5;
      float blockMidY = ITestY+.5;
      if (hitValue == 3) {
        float vecX = blockMidX - testX;
        float vecY = blockMidY - testY;
        float dist = sqrt(vecX*vecX + vecY*vecY);
        if (dist < 0.5) {
          hitWall = 1;
        }
      }
      else if (hitValue != 0) {
        hitWall = 1;

        float testPointX = playerPos.x + eyeX * (*d);
        float testPointY = playerPos.y + eyeY * (*d);

        float testAngle = atan2f((testPointY-blockMidY), (testPointX-blockMidX));
        
        if (testAngle >= -PI * 0.25f && testAngle <   PI * 0.25f) sampleX = testPointY - (float)ITestY;
				if (testAngle >=  PI * 0.25f && testAngle <   PI * 0.75f) sampleX = testPointX - (float)ITestX;
				if (testAngle <  -PI * 0.25f && testAngle >= -PI * 0.75f) sampleX = testPointX - (float)ITestX;
				if (testAngle >=  PI * 0.75f || testAngle <  -PI * 0.75f) sampleX = testPointY - (float)ITestY;

        //return hitValue;
      }

      if (hitValue != 0) {
        
        DrawLine(playerPos.x*mapScale, playerPos.y*mapScale, testX*mapScale, testY*mapScale, (Color){255,255,255,128});
        //DrawPixel(testX*mapScale, testY*mapScale, RED);
      }
    }
    /*for (int i = 0; i < 3; i++)
    {
      float vecX = sprites[i].position.x - testX;
      float vecY = sprites[i].position.y - testY;
      float dist = sqrt(vecX*vecX + vecY*vecY);
      if (dist < 0.25) {
        hitWall = -1;
      }
    }*/
    
  }
  return hitValue;
}
int cursorDisabled = 1;
int main()
{
  sprites[0] = (sprite){(Vector2){5.5, 2.5}, billboardSpriteAspectRatio, {billboardSprite}};

  sprites[1] = (sprite){(Vector2){10.5, 2.5}, billboardSpriteAspectRatio, {billboardSprite}};
  sprites[2] = (sprite){(Vector2){10.5, 5.5}, billboardSpriteAspectRatio, {billboardSprite}};
  
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetConfigFlags(FLAG_WINDOW_MAXIMIZED);

  InitWindow(screenWidth, screenHeight, "raylib");
  Image image = GenImageColor(bufferWidth, bufferHeight, BLUE);
  screenBuffer = LoadTextureFromImage(image);
  UnloadImage(image);
  pixels = malloc(bufferHeight * bufferWidth * sizeof(Color));

  UpdateBuffer();

  SetTargetFPS(60);
  DisableCursor();
  while (!WindowShouldClose())
  {
    UpdateDrawFrame();
    //bufferWidth = GetScreenWidth()/2;
    //bufferHeight = GetScreenHeight()/2;
  }

  CloseWindow();
  UnloadTexture(screenBuffer);
  free(pixels);

  return 0;
}

static void UpdateDrawFrame(void)
{
  Vector2 playerPosBeforeMoving = playerPos;
  // moving forwards and backwards
  if (IsKeyDown(KEY_W))
  {
    playerPos.x += playerDelta.x;
    playerPos.y += playerDelta.y;

  }
  if (IsKeyDown(KEY_S))
  {
    playerPos.x -= playerDelta.x;
    playerPos.y -= playerDelta.y;
    
  }
  // strafing left and right
  if (IsKeyDown(KEY_A))
  {
    playerPos.x -= -playerDelta.y;
    playerPos.y -= playerDelta.x;
  }
  if (IsKeyDown(KEY_D))
  {
    playerPos.x -= playerDelta.y;
    playerPos.y -= -playerDelta.x;
  }
  // turing left and right
  if (IsKeyDown(KEY_RIGHT))
  {
    playerAngle += 0.1f;
  }
  if (IsKeyDown(KEY_LEFT))
  {
    playerAngle -= 0.1f;
  }
  float angleDelta = GetMouseDelta().x/(float)GetScreenWidth();
  angleDelta *= 2;
  playerAngle += angleDelta;
  if (angleDelta < 0)
    if (playerAngle < 0)
      playerAngle = 2 * PI;

  if (angleDelta > 0)
    if (playerAngle > 2 * PI)
      playerAngle = 0;


  float speed = 0.1;
  playerDelta.x = cos(playerAngle) * speed;
  playerDelta.y = sin(playerAngle) * speed;

  if (map[(int)playerPos.y * mapSize + (int)playerPos.x] != 0) {
    playerPos = playerPosBeforeMoving;
  }
  fov += GetMouseWheelMove()/100;
  if (IsKeyPressed(KEY_SPACE) && cursorDisabled) {
    EnableCursor();
    cursorDisabled = 0;
  } 
  else if (IsKeyPressed(KEY_SPACE) && !cursorDisabled) {
    DisableCursor();
    cursorDisabled = 1;
  } 
  BeginDrawing();

  ClearBackground(RAYWHITE);

  // DrawText("This is a raylib example", 10, 40, 20, DARKGRAY);
  DrawTexturePro(screenBuffer, (Rectangle){0, 0, screenBuffer.width, screenBuffer.height}, (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()}, (Vector2){0, 0}, 0, WHITE);

  int x = 0, y = 0;
  DrawRectangle(0,0,mapSize*mapScale,mapSize*mapScale,BLACK);
  for (int i = 0; i < mapSize*mapSize; i++)
  {
    Color col = DARKGRAY;
    if (map[i] == 0) goto skipRender;
    
    if (map[i] == 3) DrawCircle(x * mapScale +mapScale/2, y * mapScale +mapScale/2, mapScale/2, col);
    else DrawRectangle(x * mapScale, y * mapScale, mapScale, mapScale, col);
    
    skipRender:
    x++;
    if (x == mapSize)
    {
      y++;
      x = 0;
    }
  }
  UpdateBuffer();
  for (int i = 0; i < 3; i++)
  {
    DrawCircle(sprites[i].position.x * mapScale, sprites[i].position.y * mapScale, mapScale / 8, GREEN);
  }
  // DrawRectangle((playerPos.x-scale/2)*scale, (playerPos.y-scale/2)*scale, scale/2,scale/2,BLUE);
  DrawCircle(playerPos.x * mapScale, playerPos.y * mapScale, mapScale / 4, BLUE);

  DrawLine(playerPos.x * mapScale, playerPos.y * mapScale, (playerPos.x + playerDelta.x / speed) * mapScale, (playerPos.y + playerDelta.y / speed) * mapScale, BLUE);

  DrawFPS(screenWidth - 100, 10);
  EndDrawing();
}