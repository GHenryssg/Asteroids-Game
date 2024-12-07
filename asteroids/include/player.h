#ifndef PLAYER_H
#define PLAYER_H

#include "../include/constants.h"
#include "../include/bullet.h"
#include "../include/asteroid.h"


#define PLAYER_SYMBOL 2

extern vida1;
extern vida2;

typedef struct {
    int x, y;
    int dx, dy;
    int speed;
    int health;
    int active;
    int id;
    //int limite_bullets;
} Player;

void inicializePlayer(Player *player, int num);

void movePlayer(Player *player, int map[HEIGHT][WIDTH]);

void updatePlayerOnMap(int map[HEIGHT][WIDTH], Player *player);

//void restartGame(Player *player1, Player *player2, int map[HEIGHT][WIDTH], Asteroid asteroids[], Bullet b);

#endif
