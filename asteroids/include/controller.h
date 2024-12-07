#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <pthread.h>

typedef struct {
    Bullet *bullets;
    Asteroid *asteroids;
    int maxBullets;
    int maxAsteroids;
    int (*map)[WIDTH];
} CollisionContext;



// Declarações de variáveis globais
extern int gameState;  // 0 = MENU, 1 = PLAYING, 2 = PAUSED
extern int running;    // Controle se o jogo está rodando
extern int asteroidss;
extern int tiro;
extern pthread_mutex_t inputMutex;
extern pthread_mutex_t collisionMutex;

// Funções de controle de fluxo
void *inputThread(void *arg);
void displayMenu();
void displayPauseMenu();
int kbhit(void);  // Função para detectar se há entrada de teclado sem bloqueio
int checkBulletCollision(Bullet *bullet, Asteroid *asteroid);

#endif