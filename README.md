# 🚀 Asteroid Antics

**Asteroid Antics** is a terminal-based space survival game written in C. The player pilots a spaceship through an asteroid field, collecting space junk to refuel while avoiding collisions and managing limited fuel. The game features a leaderboard, multiple difficulty modes, and a text-based world rendered directly in the terminal.

## 🧠 Concept

- Navigate through a dynamically updating space grid.
- Avoid moving asteroids (`A`) and collect space junk (`J`) to increase your fuel and score.
- The game ends when the player runs out of fuel, crashes into an asteroid, or successfully collects five pieces of junk.
- Players are scored based on the amount of junk collected and remaining fuel.

## 🎮 Controls

- `w` – Move up  
- `a` – Move left (increases fuel slightly)  
- `s` – Move down  
- `d` – No movement (placeholder)  
- `q` – Move right

## 🏁 Game Modes

After launching the game, you'll be prompted to select a difficulty:

- **0** – Easy (More fuel, fewer asteroids)
- **1** – Hard (Less fuel, more asteroids)

## 🛠 Compilation

You’ll need a C compiler like `gcc`. To compile:

```bash
gcc -o asteroid_antics main.c game.c -std=c99
