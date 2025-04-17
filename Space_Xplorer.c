#include "game.h"
FILE *fptr;

void init(World *world);    //Initialise player position and world
void print_grid(const World *world);
void print_asteroids(World *world);
void update(World *world);
void instructions();

int main(void) {

    World world = {};
    world.asteroids = ASTEROIDS;
    player_data player_data = {};

    instructions();

    init (&world);
    print_grid (&world);
    //printf("Hello, World!\n");
    return 0;
}
void instructions() {
    printf("Welcome to XPLORER!\n");
    printf("Press any key to continue...");
    getchar();
    printf("\n");
    fptr = fopen("Instructions.txt", "r");
    char instructions[100];
    while(fgets(instructions, 100, fptr )) {
        printf("%s", instructions);
    }
    fclose(fptr);// Close the file
}
void init(World *world) {                       //function to initialise the world or reset if needed
    for (int i = 0; i < WORLD_SIZE_X; i++) {
        for (int j = 0; j < WORLD_SIZE_Y; j++) {
            world->grid[i][j] = 'O';
        }
    }
}
void print_grid(const World *world) {
    for (int i = 0; i < WORLD_SIZE_X; i++) {
        for (int j = 0; j < WORLD_SIZE_Y; j++) {
            printf("    %c", world->grid[i][j]);
        }
        printf ("\n");
    }
}
void print_asteroids(World *world) {}
void update(World *world) {}