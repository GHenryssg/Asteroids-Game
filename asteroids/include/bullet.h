#ifndef BULLET_H
#define BULLET_H

#include "../include/player.h"
#include "../include/constants.h"


typedef struct {
    int x, y;  // Posição da bullet
    int dx, dy;       
    int active;     // Flag para indicar se a bullet está ativa
} Bullet;

// Funções para gerenciar bullets
void initializeBullets(Bullet bullets[], int maxBullets);

void shootBullet(Bullet bullets[], int maxBullets, Player *player);

void updateBullets(Bullet bullets[], int maxBullets, int map[HEIGHT][WIDTH]);

void drawBullets(Bullet bullets[], int maxBullets, int map[HEIGHT][WIDTH]);

#endif