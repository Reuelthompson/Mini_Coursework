/*  Filename   : Asteroid Antics
    Description: A space travel game where the player avoids asteroids, collects space junk, and manages fuel.
    Author     : Reuel Thompson
    Date       : 27/04/2025
    Input      : Player name, ship name, movement commands
    Output     : Game grid display, score, leaderboard updates
    Version    : 1.0 */
//------------------------------------------------Includes--------------------------------------------------------------
#include "game.h"
//-----------------------------------------------Prototypes-------------------------------------------------------------
void world_init(World *world);
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

int random_number_generator();

void player_name(player_data *player);
void player_data_init(player_data *player, const int *game_mode);
void player_data_update(player_data *player);
void print_player_data(player_data *player);
void player_world(World *world, const player_data *player);
void player_move(player_data *player, World *world);

bool collision_detection(World *world, const player_data *player);
void space_junk_collected(const World *world, player_data *player);
void score_calculation(World *world, player_data *player);

int instructions();
int load_score_board();
void sort_score_board(int count);
void save_score_board(int count);
//---------------------------------------------------------Main---------------------------------------------------------
int main(void) {

    World world = {};                           //initialise structs
    player_data player = {};

    srand(time(NULL));                     //Seed random number generator
    bool end = true;                            //Initialise variable end for the loop function of the game

    int game_mode = instructions();             //Get game mode difficulty and print the instructions from the txt file

    world_init (&world);                        //Build world
    asteroids_init (&world);                    //Add asteroids
    space_junk_init (&world);                   //Add space junk

    player_name(&player);                       //Get player name
    player_data_init (&player, &game_mode);     //Sets score, position, and fuel
    player_world(&world, &player);              //Places player in world

    printf("\nPress any key to start...\n");
    getchar();
    printf("\n");

    do {                                            //Do while loop
        print_grid(&world);                         //Prints world
        player_move(&player, &world);               //Moves player coordinates
        printf("\n %d\n", player.fuel);       //Prints remaining fuel
        World_update(&world, &player);              //Moves all asteroids and space junk and creates new space
        space_junk_collected(&world, &player);      //Check if player is on space junk
        end = collision_detection(&world, &player); //Check if player has hit an asteroid
        if (player.fuel <= 0) {                     //Ends game if fuel reaches 0
            end = false;
        }
    } while (end);

    print_grid(&world);                         //Print world with 'D' instead of 'P'
    score_calculation(&world, &player);         //Calculates score
    print_player_data(&player);                 //Prints the player's data for testing purposes
    printf("End of the game!\n");

    int player_count = load_score_board();      //Loads the scoreboard and returns the number of players on the score board
    leader_board[player_count] = player;        //puts the number of people in the score board into an array
    player_count++;                             //Adds the current player
    sort_score_board(player_count);             //Sorts the scores highest to lowest
    if (player_count > TOP_SCORES) {            //Saves only the top 10 scores
        player_count = TOP_SCORES;
    }
    save_score_board(player_count);             //Writes to the Leader_board file
    return 0;                                   //End of program
}
//--------------------------------------------Functions-----------------------------------------------------------------
int instructions() {
    // File pointer for reading the instructions file
    printf("\nWelcome to Asteroid Antics!\n");
    printf("Press any key to continue...");
    getchar();                                                      // Wait for user input to proceed
    printf("\n");                                             // New line for spacing

    FILE *fptr = fopen("Instructions.txt", "r");            // Open the instructions file in read mode
    char instructions[100];                                         // Buffer to store each line of the instructions
    while(fgets(instructions, 100, fptr )) {               // Read and print each line until end of file
        printf("%s", instructions);                           // Print the current line of instructions
    }
    int game_mode = 0;                                              // Initialize game mode variable
    fclose(fptr);                                                   // Close the instructions file

    scanf("%d", &game_mode);                                 // Get the player's chosen game mode (easy/hard)
    return game_mode;                                               // Return the selected game mode
}
//--------------------------------------------Player functions----------------------------------------------------------
void player_name(player_data *player) {
    printf("\nEnter Username: \n");
    scanf("%s", &player->person.user_name);
    printf("Enter Ship name: \n");
    scanf("%s", &player->person.ship_name);
}
void player_data_init(player_data *player, const int *game_mode) {    // Function to initialize the player data
    player->location.x           = 0;                                 // Set initial x-coordinate of player location
    player->location.y           = 9;                                 // Set initial y-coordinate of player location
    player->score                = 0;                                 // Initialize player score to 0
    if (game_mode == 0) {                                             // Check if game mode is easy
        player->fuel           = 100;                                 // Set fuel to 100 for easy mode
    } else {                                                          // If game mode is hard
        player->fuel            = 50;                                 // Set fuel to 50 for hard mode
    }
    player->space_junk_collected = 0;                                 // Initialize space junk collected to 0
}
void print_player_data(player_data *player) {                       //function for testing, to see what data is stored
    printf("\nUsername: %s\n", player->person.user_name);
    printf("Ship name: %s\n", player->person.ship_name);
    printf("x_coordinate: %d\n", player->location.x);
    printf("y_coordinate: %d\n", player->location.y);
    printf("score: %d\n", player->score);
    printf("fuel: %d\n", player->fuel);
    printf("space_junk_collected: %d\n", player->space_junk_collected);
}
void player_world(World *world, const player_data *player) {    //Function places the player in the world
    for (int y = 0; y < WORLD_SIZE_X; y++) {                    //Improvement: doesn't need to be a nested for loop
        for (int x = 0; x < WORLD_SIZE_Y; x++) {
            if (player->location.x == x && player->location.y == y) {
                world->grid[y][x] = 'P';
            }
        }
    }
}
void player_move(player_data *player, World *world) {    // Function to handle player movement in the world
    const int x = player->location.x;                    // Store current x-coordinate of the player
    const int y = player->location.y;                    // Store current y-coordinate of the player
    world->grid[y][x] = ' ';                             // Clear the current position on the world grid
    char direction;                                      // Variable to store the player's movement input
    printf("What is your next move?\n");           // Prompt player for movement input
    scanf(" %c", &direction);                      // Read the direction input (w, a, s, d, or q)

    switch (direction) {                                 // Handle movement based on the input
        case 'w':                                        // If 'w' is pressed, move up (decrease y)
            player->location.y = player->location.y - 1; // Update player's y-coordinate
            player->fuel--;                              // Decrease fuel for movement
            break;
        case 'a':                                        // If 'a' is pressed, move left (decrease x)
            if (player->location.x != 0) {               // Check if the player is not at the leftmost edge
                player->location.x = player->location.x - 1;  // Update player's x-coordinate
                player->fuel++;                          // Increase fuel for movement (moving left costs less fuel)
            }
            break;
        case 's':                                         // If 's' is pressed, move down (increase y)
            player->location.y = player->location.y + 1;  // Update player's y-coordinate
            player->fuel--;                               // Decrease fuel for movement
            break;
        case 'd':                                         // If 'd' is pressed, move right (increase x)
            player->location.x = player->location.x + 1;  // Update player's x-coordinate
            player->fuel--;                               // Decrease fuel for movement
            break;
        case 'q':                                         // If 'q' is pressed, do nothing
            break;
        default:                                          // If any other key is pressed, do nothing
            break;
    }
}
bool collision_detection(World *world, const player_data *player) {    // Function to detect if the player has collided with an asteroid or gone out of bounds
    const int x = player->location.x;                                  // Store the player's current x-coordinate
    const int y = player->location.y;                                  // Store the player's current y-coordinate
    // Check if the player is out of bounds (i.e., outside the grid)
    if (x < 0 || x >= WORLD_SIZE_X || y < 0 || y >= WORLD_SIZE_Y) {    // If player's position is outside the grid
        return false;                                                  // Return false (indicating collision or out-of-bounds)
    }
    // Check if the player has collided with an asteroid ('A')
    if (world->temp_grid[y][x] == 'A') {                                // If the position in the temporary grid contains an asteroid
        world->grid[y][x] = 'D';                                        // Mark the player's position with 'D' (death) on the world grid
        return false;                                                   // Return false (collision with asteroid)
    }
    return true;                                                        // Return true (no collision)
}
void space_junk_collected(const World *world, player_data *player) {    // Function to check if the player has collected space junk
    const int x = player->location.x;                                   // Store the player's current x-coordinate
    const int y = player->location.y;                                   // Store the player's current y-coordinate

    // Check if the player is on a space junk ('J') on the temporary grid
    if (world->temp_grid[y][x] == 'J') {                                // If there's space junk at the player's position
        player->space_junk_collected += 1;                              // Increase the player's space junk collected by 1
    }
}
void score_calculation(World *world, player_data *player) {             // Function to calculate the player's score based on collected junk and remaining fuel
    player->score = player->space_junk_collected * 10 - player->fuel;   // Score = space junk collected minus the remaining fuel
}
//----------------------------------------------World functions---------------------------------------------------------
void world_init(World *world) {                     // Function to initialize or reset the world grid
    world->asteroids = ASTEROIDS;                   // Set the number of asteroids in the world
    world->space_junk = SPACE_JUNK;                 // Set the number of space junk items in the world
    for (int i = 0; i < WORLD_SIZE_X; i++) {        // Loop through the grid's x-dimension
        for (int j = 0; j < WORLD_SIZE_Y; j++) {    // Loop through the grid's y-dimension
            world->grid[i][j] = ' ';                // Initialize each grid position as empty (' ')
        }
    }
}

void asteroids_init(World *world) {                             // Function to initialize asteroids in the world
    for (int y = 0; y < WORLD_SIZE_Y; y++) {                    // Loop through the grid's y-dimension
        for (int x = 0; x < WORLD_SIZE_X; x++) {                // Loop through the grid's x-dimension
            int asteroid_seed = random_number_generator();      // Generate a random number for asteroid placement
            // Check if there's space for an asteroid, and it doesn't collide with other objects
            if (asteroid_seed <= 15 && x > 3 && world->asteroids != 0) {
                if ((world->grid[y][x+1] != 'A' || 'J') && (world->grid[y+1][x] != 'A' || 'J') &&
                    (world->grid[y][x-1] != 'A' || 'J') && (world->grid[y-1][x] != 'A' || 'J') &&
                    (world->grid[y-1][x-1] != 'A' || 'J') && (world->grid[y+1][x+1] != 'A' || 'J')) {
                    world->grid[y][x] = 'A';        // Place asteroid on the grid
                    world->temp_grid[y][x] = 'A';   // Also mark it in the temporary grid
                    world->asteroids--;             // Decrease the number of remaining asteroids
                }
            }
        }
    }
}

void space_junk_init(World *world) {                 // Function to initialize space junk in the world
    for (int y = 0; y < WORLD_SIZE_Y; y++) {         // Loop through the grid's y-dimension
        for (int x = 0; x < WORLD_SIZE_X; x++) {     // Loop through the grid's x-dimension
            int space_junk_seed = random_number_generator(); // Generate a random number for space junk placement
            // Check if space junk can be placed and not collide with asteroids or other junk
            if (space_junk_seed <= 5 && x > 3 && world->space_junk != 0 &&
                (world->grid[y][x+1] != 'A' || 'J') &&
                (world->grid[y+1][x] != 'A' || 'J') &&
                (world->grid[y][x-1] != 'A' || 'J') &&
                (world->grid[y-1][x] != 'A' || 'J') &&
                (world->grid[y-1][x-1] != 'A' || 'J') &&
                (world->grid[y+1][x+1] != 'A' || 'J')) {
                world->grid[y][x] = 'J';        // Place space junk on the grid
                world->temp_grid[y][x] = 'J';   // Also mark it in the temporary grid
                world->space_junk--;            // Decrease the number of remaining space junk
            }
        }
    }
}

void asteroid_update(World *world) {                 // Function to update asteroids' positions
    for (int y = 0; y < WORLD_SIZE_X; y++) {         // Loop through the grid's y-dimension
        for (int x = 0; x < WORLD_SIZE_Y; x++) {     // Loop through the grid's x-dimension
            if (world->temp_grid[y][x] == 'A') {     // Check if there is an asteroid in the temporary grid
                world->temp_grid[y][x-1] = 'A';      // Move the asteroid one step left in the temporary grid
                world->temp_grid[y][x] = ' ';        // Clear the current position
            }
        }
    }
}

void space_junk_update(World *world) {               // Function to update space junk's positions
    for (int y = 0; y < WORLD_SIZE_Y; y++) {         // Loop through the grid's y-dimension
        for (int x = 0; x < WORLD_SIZE_X; x++) {     // Loop through the grid's x-dimension
            if (world->temp_grid[y][x] == 'J') {     // Check if there is space junk in the temporary grid
                world->temp_grid[y][x-1] = 'J';      // Move the space junk one step left in the temporary grid
                world->temp_grid[y][x] = ' ';        // Clear the current position
            }
        }
    }
}

void new_space(World *world) {                       // Function to clear the last row in the temporary grid
    for (int y = 0; y < WORLD_SIZE_Y; y++) {         // Loop through the grid's y-dimension
        for (int x = 0; x < WORLD_SIZE_X; x++) {     // Loop through the grid's x-dimension
            if (y == WORLD_SIZE_X - 1) {             // If it's the last row
                world->temp_grid[y][x] = ' ';        // Clear the position
            }
        }
    }
}

void new_asteroids(World *world) {                   // Function to generate new asteroids in the last row
    for (int y = 0; y < WORLD_SIZE_Y; y++) {         // Loop through the grid's y-dimension
        for (int x = 0; x < WORLD_SIZE_X; x++) {     // Loop through the grid's x-dimension
            if (y == WORLD_SIZE_X - 1) {             // If it's the last row
                int asteroid_seed = random_number_generator(); // Generate a random number for asteroid placement
                if (asteroid_seed <= 15 && world->grid[x][y] != 'J') {  // Avoid placing asteroids on junk
                    world->grid[x][y] = 'A';         // Place an asteroid on the grid
                }
            }
        }
    }
}

void new_space_junk(World *world) {                  // Function to generate new space junk in the last row
    for (int y = 0; y < WORLD_SIZE_Y; y++) {         // Loop through the grid's y-dimension
        for (int x = 0; x < WORLD_SIZE_X; x++) {     // Loop through the grid's x-dimension
            if (y == WORLD_SIZE_X - 1) {             // If it's the last row
                int space_junk_seed = random_number_generator(); // Generate a random number for space junk placement
                if (space_junk_seed <= 5 && world->grid[x][y] != 'A') {  // Avoid placing space junk on asteroids
                    world->grid[x][y] = 'J';         // Place space junk on the grid
                }
            }
        }
    }
}

void World_update(World *world, const player_data *player) { // Function to update the world each turn
    for (int y = 0; y < WORLD_SIZE_Y; y++) {                 // Loop through the grid's y-dimension
        for (int x = 0; x < WORLD_SIZE_X; x++) {             // Loop through the grid's x-dimension
            world->temp_grid[x][y] = world->grid[x][y];      // Copy current grid to temp grid
        }
    }
    asteroid_update(world);                             // Update asteroid positions
    for (int y = 0; y < WORLD_SIZE_Y; y++) {            // Loop through the grid's y-dimension
        for (int x = 0; x < WORLD_SIZE_X; x++) {        // Loop through the grid's x-dimension
            world->grid[x][y] = world->temp_grid[x][y]; // Copy temp grid back to the main grid
        }
    }
    space_junk_update(world);                            // Update space junk positions
    for (int y = 0; y < WORLD_SIZE_Y; y++) {             // Loop through the grid's y-dimension
        for (int x = 0; x < WORLD_SIZE_X; x++) {         // Loop through the grid's x-dimension
            world->grid[x][y] = world->temp_grid[x][y] ; // Copy temp grid back to the main grid
        }
    }
    new_space(world);                                  // Clear the last row in the temp grid
    new_asteroids(world);                              // Add new asteroids to the last row
    new_space_junk(world);                             // Add new space junk to the last row
    player_world(world, player);                       // Update the player's position in the world
}

void print_grid(const World *world) {                  // Function to print the current grid
    printf("\n\n");
    for (int y = 0; y < WORLD_SIZE_X; y++) {           // Loop through the grid's y-dimension
        for (int x = 0; x < WORLD_SIZE_Y; x++) {       // Loop through the grid's x-dimension
            printf("    %c", world->grid[y][x]); // Print each grid element
        }
        printf("\n");
    }
}

void print_temp_grid(const World *world) {                  // Function to print the temporary grid
    printf("\n\n");
    for (int y = 0; y < WORLD_SIZE_X; y++) {                // Loop through the grid's y-dimension
        for (int x = 0; x < WORLD_SIZE_Y; x++) {            // Loop through the grid's x-dimension
            printf("    %c", world->temp_grid[y][x]); // Print each temporary grid element
        }
        printf("\n");
    }
}

int random_number_generator() {                        // Function to generate a random number between 1 and 100
    int random_number = (rand() % 100) + 1;            // Generate a random number between 1 and 100
    return random_number;                              // Return the generated number
}

//-----------------------------------------------Score_board------------------------------------------------------------
int load_score_board() {                               // Function to load the scoreboard from file
    FILE *fptr = fopen("Leader_board.txt", "r");        // Open leaderboard file for reading
    int count = 0;
    if (fptr == NULL) return 0;                         // Return 0 if the file can't be opened
    while (fscanf(fptr, "%s %s %d", leader_board[count].person.user_name, leader_board[count].person.ship_name,
        &leader_board[count].score) == 3) {             // Read each entry (username, ship name, score)
        count++;                                        // Increase the count for leaderboard entries
        if (count >= TOP_SCORES) break;                 // Stop if the leaderboard is full
    }
    fclose(fptr);                                       // Close the file
    return count;                                       // Return the number of entries loaded
}

void sort_score_board(int count) {                      // Function to sort the leaderboard in descending order
    for (int i = 0; i < count - 1; i++) {               // Loop through the leaderboard
        for (int j = i + 1; j < count; j++) {           // Loop through the leaderboard
            if (leader_board[j].score > leader_board[i].score) { // Compare scores and swap if needed
                player_data temp = leader_board[i];
                leader_board[i] = leader_board[j];
                leader_board[j] = temp;
            }
        }
    }
}

void save_score_board(int count) {                              // Function to save the leaderboard to file
    FILE *file = fopen("Leader_board.txt", "w");  // Open leaderboard file for writing
    if (file == NULL) {                                         // If file can't be opened, print error
        perror("Error saving scoreboard");
        return;
    }
    for (int i = 0; i < count; i++) {                   // Loop through the leaderboard
        fprintf(file, "%s %s %d\n", leader_board[i].person.user_name, leader_board[i].person.ship_name, leader_board[i].score); // Write each entry
    }
    fclose(file);                                       // Close the file after saving
}
