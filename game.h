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
#define ASTEROIDS    20
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

typedef struct {
    int asteroids;
    char grid[WORLD_SIZE_X][WORLD_SIZE_Y];
}World;

//------------------------------------------------
#endif //GAME_H
