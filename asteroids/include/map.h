#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include "../include/constants.h"
#include "../include/asteroid.h"
#include "../include/bullet.h"
#include "../include/player.h"

int tiro = 0;
int asteroidss =0;
int jogador = 0;

void createMap(int map[HEIGHT][WIDTH]);

void printMap(int map[HEIGHT][WIDTH], Asteroid ast[MAX_ASTEROIDS], Bullet b[MAX_BULLETS], Player *player1, Player *player2);

void processInput(Player *player, Bullet bullets[]);

void apaga_bala();
#endif