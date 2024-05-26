#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"

#define GRUBOX_BACKGROUND CLITERAL(Color){ 28, 28, 28, 255 }
#define GRUBOX_YELLOW CLITERAL(Color){ 250, 189, 47, 255 }
#define GRUBOX_GREEN CLITERAL(Color){ 184, 187, 38, 100}
#define THICK 2


#define LIST_INIT_CAP 256
#define list_append(list, item)                                               \
    do {                                                                       \
        if ((list)->count >= (list)->total) {                                   \
            (list)->total = (list)->total == 0 ? LIST_INIT_CAP : (list)->total*2;\
            (list)->items = realloc((list)->items, (list)->total*sizeof(*(list)->items)); \
            assert((list)->items != NULL && "Buy more RAM lol");                 \
        }                                                                         \
        (list)->items[(list)->count++] = (item);                                   \
    } while (0)


typedef struct {
    int *items;
    size_t count;
    size_t total;
} Recaman ;

typedef struct {
  Vector2 start;
  Vector2 end;
} Line ;

float fmap(float n, float start1, float stop1, float start2, float stop2) {
  return ((n-start1)/(stop1-start1))*(stop2-start2)+start2;
};

void finder(int *current, int *counter, Recaman *recaman){

  for (size_t i = 0; i < recaman->count; i++) {
    if (*current-*counter == recaman->items[i]){
      *current += *counter;
      *counter = *counter+1;
      list_append(recaman, *current);
      return;
    }
  }

  if (*current-*counter<0){
    *current += *counter;
    *counter = *counter+1;
  } else {
    *current -= *counter;
    *counter = *counter+1;
  }

  list_append(recaman, *current);
}

int main(void) {

  int screenWidth = 800;
  int screenHeight = 800;

  Line baseline = {
    (Vector2){0, screenHeight/2},
    (Vector2){screenWidth, screenHeight/2},
  };

  Vector2 dir = Vector2Subtract(baseline.end, baseline.start);

  float angle = Vector2LineAngle(baseline.start, baseline.end);
  angle = (angle*180)/PI;
  float len = Vector2Distance(baseline.start, baseline.end);

  int current = 0;
  int counter = 1;
  Recaman recaman = {0};
  list_append(&recaman, 0);

  int MAX[2] = {0,0};

  for (int i = 0; i<1;i++) {
    finder(&current, &counter, &recaman);
    MAX[0]=MAX[1];
    if (current>MAX[1]){
      MAX[1]=current;
      
    }
  }

  InitWindow(screenWidth, screenHeight, "Koch Fractal Snowflake");
  SetTargetFPS(60);

  float start_angle = 0;

  while (!WindowShouldClose()) {

    if (start_angle>180){
      finder(&current, &counter, &recaman);
      MAX[0] = MAX[1];
      if (current>MAX[1]) MAX[1]=current;
      start_angle = 0;
    } else {
      start_angle += 5;
    }

    BeginDrawing();

      ClearBackground(GRUBOX_BACKGROUND);
      float per =  fmap(start_angle, 0, 180, MAX[0], MAX[1]);
      Vector2 norm = Vector2Divide(dir, (Vector2){per, per});


      for (int i = 0;i<recaman.count-2;i++) {
          Vector2 center = Vector2Add(baseline.start,
                                      Vector2Scale(
                                        norm,
                                        ((recaman.items[i])+
                                        (float)(recaman.items[i+1]-recaman.items[i])/2)
                                      ));

          float radius = Vector2Length(norm) * fabs((float)recaman.items[i+1]-recaman.items[i])/2;
          // DrawCircleV((Vector2){centerX, centerY},5, ORANGE);
          if (i%2==0){
            DrawRing(center, radius-THICK/2, radius + THICK/2,
                                  180-angle, 0-angle, 100, ORANGE);
          } else {
            DrawRing(center, radius-THICK/2, radius + THICK/2,
                                  -180-angle, 0-angle, 100, ORANGE);
          }
      }

      {
        int last[2] = {recaman.items[recaman.count-2],recaman.items[recaman.count-1]};

          Vector2 center = Vector2Add(baseline.start,
                                      Vector2Scale(
                                        norm,
                                        (last[0]+(float)(last[1]-last[0])/2)
                                      ));

        float radius = Vector2Length(norm) * fabs((float)last[1]-last[0])/2;

        if ((recaman.count)%2==0){
          if (last[1]>last[0]){
            DrawRing(center, radius-THICK/2, radius + THICK/2,
                                  180-angle , 180-angle-start_angle, 100, ORANGE);
          } else {
            DrawRing(center, radius-THICK/2, radius + THICK/2,
                                  0-angle,0-angle+start_angle, 100, ORANGE);
          }
        } else {
          if (last[1]>last[0]){
            DrawRing(center, radius-THICK/2, radius + THICK/2,
                                180-angle, 180-angle+start_angle, 100, ORANGE);
          } else {
            DrawRing(center, radius-THICK/2, radius + THICK/2,
                                 0-angle, 0-angle -start_angle, 100, ORANGE);
          }
        }
      }

      DrawLineEx(baseline.start,
                 baseline.end,
                 1, GRUBOX_GREEN);

    EndDrawing();

  }

  return 0;

}
