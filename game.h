//
// Created by Reuel on 03/04/2025.
//

#ifndef GAME_H
#define GAME_H
//-------------------Global Libraries------------------------------
#include <stdio.h>
#include <stdlib.h>
//-------------------Global defines--------------------------------
#define WORLD_SIZE_X 18
#define WORLD_SIZE_Y 18
#define ASTEROIDS    20
#define SPACE_JUNK   10
#define NAME_LENGTH  20
//-------------------Global structures-----------------------------
typedef struct{
  int x;
  int y;
}coordinates;

typedef struct {
    char First_name[NAME_LENGTH];   //initialise variables inside a structure
    char Initial;
    char Surname[NAME_LENGTH];
}person_name;                       //Using Typdef to simplify Syntax further on

typedef struct{
  coordinates location;
  person_name person;
  int fuel;
  int space_junk_collected;
  int score;
 }player_data;

typedef struct {
    int asteroids;
    int space_junk;
    char grid[WORLD_SIZE_X][WORLD_SIZE_Y];
}World;

//------------------------------------------------
#endif //GAME_H
