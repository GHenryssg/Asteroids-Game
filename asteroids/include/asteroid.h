#ifndef ASTEROID_H
#define ASTEROID_H

#include "constants.h"

typedef struct {
    int x, y;
    int dx, dy;
    int shape;
    int active;
    int counter;
    int health;
} Asteroid;

void creatAsteroid(Asteroid *asteroid);

void updateAsteroid(Asteroid asteroid[MAX_ASTEROIDS], int numAsteroids, int map[HEIGHT][WIDTH]);

void hitPlayer(Asteroid *asteroid, int playerX, int playerY );

void handleBulletCollision(Asteroid *asteroid);

void initializeAsteroids(Asteroid asteroids[MAX_ASTEROIDS]);

#endif