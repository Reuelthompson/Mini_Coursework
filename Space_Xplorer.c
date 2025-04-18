#include "game.h"
FILE *fptr;

void World_init(World *world);    //Initialise player position and world
void print_grid(const World *world);
void World_update(World *world, player_data *player);
void asteroid_update(World *world);

void player_name(player_data *player);
void player_data_init(player_data *player);
void player_data_update(player_data *player);
void print_player_data(player_data *player);
void player_world(World *world, const player_data *player);
void player_move(player_data *player);

void instructions();

int main(void) {

    instructions();

    World world = {};
    World_init (&world);

    player_data player = {};
    player_name(&player);
    player_data_init (&player);
    print_player_data(&player);
    player_world(&world, &player);

    printf("\nPress any key to start...  \n");
    getchar();
    printf("\n");


    print_grid (&world);
    //printf("Hello, World!\n");
    return 0;
}
void instructions() {
    printf("\nWelcome to XPLORER!\n");
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
void player_name(player_data *player) {
    printf("\nEnter First Name: \n");
    scanf("%s", &player->person.First_name);
    printf("Enter Initial: \n");
    scanf(" %c", &player->person.Initial);
    printf("Enter Surname: \n");
    scanf("%s", &player->person.Surname);
}
void player_data_init(player_data *player) {    //function to initialise the player data
    player->location.x           = 0;
    player->location.y           = 9;
    player->score                = 0;
    player->fuel               = 100;
    player->space_junk_collected = 0;
}
void print_player_data(player_data *player) {   //function for testing, to see  what data is stored
    printf("\nInitial: %c\n", player->person.Initial);
    printf("Surname: %s\n", player->person.Surname);
    printf("x_coordinate: %d\n", player->location.x);
    printf("y_coordinate: %d\n", player->location.y);
    printf("score: %d\n", player->score);
    printf("fuel: %d\n", player->fuel);
    printf("space_junk_collected: %d\n", player->space_junk_collected);
}
void player_world(World *world, const player_data *player) {
    for (int y = 0; y < WORLD_SIZE_X; y++) {
        for (int x = 0; x < WORLD_SIZE_Y; x++) {
            if (player->location.x == x && player->location.y == y) {
                world->grid[y][x] = 'P';
            }
        }
    }
}
void player_move(player_data *player) {
    char direction;
    printf("What is your next move?\n");
    scanf("%c", &direction);
    switch (direction) {
        case 'w':
            player->location.y =+ 1;
        break;
        case 'a':
            if (player->location.x != 0) {
                player->location.x =- 1;
            }
        break;
        case 's':
            player->location.y =- 1;
        break;
        case 'd':
            player->location.x =+ 1;
        break;
        default:
            break;
    }
}
void World_init(World *world) {                  //function to initialise the world or reset if needed
    world->asteroids = ASTEROIDS;
    world->space_junk= SPACE_JUNK;
    for (int i = 0; i < WORLD_SIZE_X; i++) {
        for (int j = 0; j < WORLD_SIZE_Y; j++) {
            world->grid[i][j] = 'O';
        }
    }
}
void asteroid_update(World *world) {
    for (int i = 0; i < WORLD_SIZE_X; i++) {
        for (int j = 0; j < WORLD_SIZE_Y; j++) {
            if (world->grid[i][j] == 'A') {
                world->grid[i][j-1] = 'A';
            }
        }
    }
}
void World_update(World *world, player_data *player) {
    player_move(&player);
}
void print_grid(const World *world){           //function to print out the world
    printf ("\n");
    for (int i = 0; i < WORLD_SIZE_X; i++) {
        for (int j = 0; j < WORLD_SIZE_Y; j++) {
            printf("    %c", world->grid[i][j]);
        }
        printf ("\n");
    }
}
