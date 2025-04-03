//
// Created by Reuel on 03/04/2025.
//

#ifndef GAME_H
#define GAME_H
//------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
//------------------------------------------------
#define WORLD_SIZE_X 18
#define WORLD_SIZE_Y 18
//------------------------------------------------
typedef struct{
  int x;
  int y;
}coordinates;

typedef struct{
  coordinates location;
  int fuel;
  int space_junk;
  int score;
 }player_data;

 int world_size[WORLD_SIZE_X][WORLD_SIZE_Y];
//------------------------------------------------
#endif //GAME_H
