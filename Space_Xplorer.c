//
// Created by Reuel on 03/04/2025.
//
//-----------------------Includes-----------------------------------
#include "game.h"
//
FILE *fptr;
//----------------------Prototypes----------------------------------
void world_init(World *world);    //Initialise player position and world
void asteroids_init(World *world);
void space_junk_init(World *world);
void print_grid(const World *world);
void print_temp_grid(const World *world);
void World_update(World *world, const player_data *player);
void asteroid_update(World *world);
void space_junk_update(World *world);
void new_asteroids(World *world);
void new_space_junk(World *world);
void new_space(World *world);
void space_junk_collected(const World *world, player_data *player);

int random_number_generator();

void player_name(player_data *player);
void player_data_init(player_data *player, const int *game_mode);
void player_data_update(player_data *player);
void print_player_data(player_data *player);
void player_world(World *world, const player_data *player);
void player_move(player_data *player, World *world);

bool collision_detection(World *world, const player_data *player);
void score_calculation(World *world, player_data *player);

int instructions();
int load_score_board();
void sort_score_board(int count);
void save_score_board(int count);
//---------------------------Main----------------------------------
int main(void) {

    World world = {};
    player_data player = {};

    srand(time(NULL));                     //Seed random number generator
    bool end = true;

    int game_mode = instructions();

    world_init (&world);
    asteroids_init (&world);
    space_junk_init (&world);

    player_name(&player);
    player_data_init (&player, &game_mode);
    player_world(&world, &player);

    printf("\nPress any key to start...\n");
    getchar();
    printf("\n");

    do {
        print_grid(&world);
        player_move(&player, &world);
        printf("\n %d\n", player.fuel);
        World_update(&world, &player);
        space_junk_collected(&world, &player);
        end = collision_detection(&world, &player);
        if (player.fuel <= 0) {
            end = false;
        }
    } while (end);
    print_grid(&world);
    score_calculation(&world, &player);
    print_player_data(&player);
    printf("End of the game!\n");

    int player_count = load_score_board();
    leaderboard[player_count] = player;
    player_count++;
    sort_score_board(player_count);
    if (player_count > TOP_SCORES) {
        player_count = TOP_SCORES;
    }
    save_score_board(player_count);
    return 0;
}
//----------------------Functions----------------------------------
int instructions() {
    printf("\nWelcome to XPLORER!\n");
    printf("Press any key to continue...");
    getchar();
    printf("\n");
    fptr = fopen("Instructions.txt", "r");
    char instructions[100];
    while(fgets(instructions, 100, fptr )) {
        printf("%s", instructions);
    }
    int game_mode = 0;
    fclose(fptr);// Close the file
    scanf(" %c", &game_mode);
    return game_mode;
}
int load_score_board() {
    FILE *fptr = fopen("Leader_board.txt", "r");
    int count = 0;
    if (fptr == NULL) return 0;
    while (fscanf(fptr, "%s %s %d", leaderboard[count].person.user_name, leaderboard[count].person.ship_name, &leaderboard[count].score) == 3) {
        count++;
        if (count >= TOP_SCORES) break;
    }
    fclose(fptr);
    return count;
}
void sort_score_board(int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (leaderboard[j].score > leaderboard[i].score) {
                player_data temp = leaderboard[i];
                leaderboard[i] = leaderboard[j];
                leaderboard[j] = temp;
            }
        }
    }
}
void save_score_board(int count) {
    FILE *file = fopen("Leader_board.txt", "w");
    if (file == NULL) {
        perror("Error saving scoreboard");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %s %d\n", leaderboard[i].person.user_name, leaderboard[i].person.ship_name, leaderboard[i].score);
    }
    fclose(file);
}
void player_name(player_data *player) {
    printf("\nEnter Username: \n");
    scanf("%s", &player->person.user_name);
    printf("Enter Ship name: \n");
    scanf("%s", &player->person.ship_name);
}
void player_data_init(player_data *player, const int *game_mode) {    //function to initialise the player data
    player->location.x           = 0;
    player->location.y           = 9;
    player->score                = 0;
    if (game_mode == 0) {
        player->fuel           = 100;
    }else
        player->fuel            = 50;
    player->space_junk_collected = 0;
}
void print_player_data(player_data *player) {   //function for testing, to see  what data is stored
    printf("\nUsername: %s\n", player->person.user_name);
    printf("Ship name: %s\n", player->person.ship_name);
    printf("x_coordinate: %d\n", player->location.x);
    printf("y_coordinate: %d\n", player->location.y);
    printf("score: %d\n", player->score);
    printf("fuel: %d\n", player->fuel);
    printf("space_junk_collected: %d\n", player->space_junk_collected);
}
void player_world(World *world, const player_data *player) {    //doesn't need to be a nested for loop
    for (int y = 0; y < WORLD_SIZE_X; y++) {
        for (int x = 0; x < WORLD_SIZE_Y; x++) {
            if (player->location.x == x && player->location.y == y) {
                world->grid[y][x] = 'P';
            }
        }
    }
}
void player_move(player_data *player, World *world) {
    const int x = player->location.x;
    const int y = player->location.y;
    world->grid[y][x] = ' ';
    char direction;
    printf("What is your next move?\n");
    scanf(" %c", &direction);
    switch (direction) {
        case 'w':
            player->location.y = player->location.y - 1;
            player->fuel --;
            break;
        case 'a':
            if (player->location.x != 0) {
                player->location.x = player->location.x - 1;
                player->fuel ++;
            }
            break;
        case 's':
            player->location.y = player->location.y + 1;
            player->fuel --;
            break;
        case 'd':
            player->location.x = player->location.x + 1;
            player->fuel --;
            break;
        case'q':
            break;
        default:
            break;
    }
}
bool collision_detection(World *world, const player_data *player) {
    const int x = player->location.x;
    const int y = player->location.y;
    if (x < 0 || x >= WORLD_SIZE_X || y < 0 || y >= WORLD_SIZE_Y) {
        return false;
    }
    if (world->temp_grid[y][x] == 'A') {
        world->grid[y][x] = 'D';
        return false;
    }
    return true;
}
void space_junk_collected(const World *world, player_data *player) {
    const int x = player->location.x;
    const int y = player->location.y;
    if (world->temp_grid[y][x] == 'J') {
        player->space_junk_collected += 10;

    }
}
void score_calculation(World *world, player_data *player) {
    player->score = player->space_junk_collected - player->fuel;
}
void world_init(World *world) {                  //function to initialise the world or reset if needed
    world->asteroids = ASTEROIDS;
    world->space_junk= SPACE_JUNK;
    for (int i = 0; i < WORLD_SIZE_X; i++) {
        for (int j = 0; j < WORLD_SIZE_Y; j++) {
            world->grid[i][j] = ' ';
        }
    }
}
void asteroids_init(World *world) {
    for (int y = 0; y < WORLD_SIZE_Y; y++) {
        for (int x = 0; x < WORLD_SIZE_X; x++) {
            int asteroid_seed = random_number_generator();
            if (asteroid_seed <= 15 && x > 3 && world->asteroids != 0) {
                if ((world->grid[y][x+1] != 'A' || 'J') && (world->grid[y+1][x] != 'A' || 'J') &&
                        (world->grid[y][x-1] != 'A' || 'J') && (world->grid[y-1][x] != 'A' || 'J') &&
                            (world->grid[y-1][x-1] != 'A' || 'J') && (world->grid[y+1][x+1] != 'A' || 'J')) {
                    world->grid[y][x] = 'A';
                    world->temp_grid[y][x] = 'A';
                    world -> asteroids --;
                    }
                }
            }
        }
}
void space_junk_init(World *world) {
    for (int y = 0; y < WORLD_SIZE_Y; y++) {
        for (int x = 0; x < WORLD_SIZE_X; x++) {
            int space_junk_seed = random_number_generator();
            if (space_junk_seed <= 5 && x > 3 && world->asteroids != 0 &&(world->grid[y][x+1] != 'A' || 'J') &&
                (world->grid[y+1][x] != 'A' || 'J') && (world->grid[y][x-1] != 'A' || 'J') &&
                    (world->grid[y-1][x] != 'A' || 'J') && (world->grid[y-1][x-1] != 'A' || 'J') &&
                        (world->grid[y+1][x+1] != 'A' || 'J')) {
                world->grid[y][x] = 'J';
                world->temp_grid[y][x] = 'J';
                world->space_junk --;

            }
        }
    }
}
void asteroid_update(World *world) {
    for (int y = 0; y < WORLD_SIZE_X; y++) {
        for (int x = 0; x < WORLD_SIZE_Y; x++) {
            if (world->temp_grid[y][x] == 'A') {
                world->temp_grid[y][x-1] = 'A';
                world->temp_grid[y][x] = ' ';
            }
        }
    }
}
void space_junk_update(World *world) {
    for (int y = 0; y < WORLD_SIZE_Y; y++) {
        for (int x = 0; x < WORLD_SIZE_X; x++) {
             if (world->temp_grid[y][x] == 'J') {
                 world->temp_grid[y][x-1] = 'J';
                 world->temp_grid[y][x] = ' ';
            }
        }
    }
}
void new_space(World *world) {
    for (int y = 0; y < WORLD_SIZE_Y; y++) {
        for (int x = 0; x < WORLD_SIZE_X; x++) {
            if (y == WORLD_SIZE_X -1) {
                world->temp_grid[y][x] = ' ';
            }
        }
    }
}
void new_asteroids(World *world) {
    for (int y = 0; y < WORLD_SIZE_Y; y++) {
        for (int x = 0; x < WORLD_SIZE_X; x++) {
            if (y == WORLD_SIZE_X -1) {
                int asteroid_seed = random_number_generator();
                if (asteroid_seed <= 15 && world->grid[x][y] != 'J') {
                    world->grid[x][y] = 'A';
                }
            }
        }
    }
}
void new_space_junk(World *world) {
    for (int y = 0; y < WORLD_SIZE_Y; y++) {
        for (int x = 0; x < WORLD_SIZE_X; x++) {
            if (y == WORLD_SIZE_X -1) {
                int asteroid_seed = random_number_generator();
                if (asteroid_seed <= 5 && world->grid[x][y] != 'A') {
                    world->grid[x][y] = 'J';
                }
            }
        }
    }
}
void World_update(World *world, const player_data *player) {
    for (int y = 0; y < WORLD_SIZE_Y; y++) {
        for (int x = 0; x < WORLD_SIZE_X; x++) {
             world->temp_grid[x][y] = world->grid[x][y] ;
        }
    }
    asteroid_update(world);
    for (int y = 0; y < WORLD_SIZE_Y; y++) {
        for (int x = 0; x < WORLD_SIZE_X; x++) {
            world->grid[x][y] = world->temp_grid[x][y] ;
        }
    }
    space_junk_update(world);
    for (int y = 0; y < WORLD_SIZE_Y; y++) {
        for (int x = 0; x < WORLD_SIZE_X; x++) {
            world->grid[x][y] = world->temp_grid[x][y] ;
        }
    }
    new_space(world);
    new_asteroids(world);
    new_space_junk(world);
    player_world(world, player);
}
void print_grid(const World *world){           //function to print out the world
    printf ("\n\n");
    for (int y = 0; y < WORLD_SIZE_X; y++) {
        for (int x = 0; x < WORLD_SIZE_Y; x++) {
            printf("    %c", world->grid[y][x]);
        }
        printf ("\n");
    }
}
void print_temp_grid(const World *world){           //function to print out the world
    printf ("\n\n");
    for (int y = 0; y < WORLD_SIZE_X; y++) {
        for (int x = 0; x < WORLD_SIZE_Y; x++) {
            printf("    %c", world->temp_grid[y][x]);
        }
        printf ("\n");
    }
}
int random_number_generator() {
    int random_number = (rand()% 100) + 1;
    //printf("Random number: %d\n", random_number);
    return random_number;
}
int random_number_generator_y() {
    int random_number = (rand()% 100) + 1;
    return random_number;
}