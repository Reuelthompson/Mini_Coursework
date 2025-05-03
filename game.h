//
// Created by Reuel on 03/04/2025.
//
#ifndef GAME_H
#define GAME_H
//-------------------Global Libraries------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
//-------------------Global defines--------------------------------
#define WORLD_SIZE_X 19
#define WORLD_SIZE_Y 19
#define ASTEROIDS    90
#define SPACE_JUNK   10
#define NAME_LENGTH  20
#define TOP_SCORES   10
//-------------------Global structures-----------------------------
typedef struct{
  int x;
  int y;
}coordinates;

typedef struct {
    char user_name[NAME_LENGTH];   //initialise variables inside a structure
    char ship_name[NAME_LENGTH];
}person_name;                       //Using Typdef to simplify Syntax further on

typedef struct{
  coordinates location;
  person_name person;
  int fuel;
  int space_junk_collected;
  int spaceship_health;
  int score;
 }player_data;

typedef struct {
    int asteroids;
    int space_junk;
    char grid[WORLD_SIZE_X][WORLD_SIZE_Y];
    char temp_grid[WORLD_SIZE_X][WORLD_SIZE_Y];
}World;

//------------------------------------------------

player_data leader_board[TOP_SCORES];  // Array to hold the leaderboard

#endif //GAME_H
