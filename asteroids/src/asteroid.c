#include "../include/asteroid.h"
#include <stdlib.h>
#include <time.h>

void setAsteroidPosition(Asteroid *asteroid) {
    int side = rand() % 1;  // Escolher entre 4 lados: 0 = topo, 1 = fundo, 2 = esquerda, 3 = direita
    switch (side) {
        case 0: // Topo
            asteroid->x = rand() % WIDTH; //rand() % WIDTH;  // Aleatório na largura
            asteroid->y = 0;               // Linha do topo
            asteroid->dy = 1;              // Movimento para baixo
            asteroid->dx = (rand() % 3) - 1;//(rand() % 3) - 1; // Movimento horizontal aleatório
            break;
        case 1: // Fundo
            asteroid->x = rand() % WIDTH;  // Aleatório na largura
            asteroid->y = HEIGHT - 1;      // Linha do fundo
            asteroid->dy = -1;             // Movimento para cima
            asteroid->dx = (rand() % 3) - 1; // Movimento horizontal aleatório
            break;
        case 2: // Lado esquerdo
            asteroid->x = 0;               // Coluna da esquerda
            asteroid->y = rand() % HEIGHT; // Aleatório na altura
            asteroid->dx = 1;              // Movimento para a direita
            asteroid->dy = (rand() % 3) - 1; // Movimento vertical aleatório
            break;
        case 3: // Lado direito
            asteroid->x = WIDTH - 1;       // Coluna da direita
            asteroid->y = rand() % HEIGHT; // Aleatório na altura
            asteroid->dx = -1;             // Movimento para a esquerda
            asteroid->dy = (rand() % 3) - 1; // Movimento vertical aleatório
            break;
    }
}

void setAsteroidDirection(Asteroid *asteroid) {
    // Se o asteroide nasceu no topo, ele só pode mover para baixo
    if (asteroid->y == 0) {
        asteroid->dy = 1;  // Só pode mover para baixo
    }
    // Se o asteroide nasceu no fundo, ele só pode mover para cima
    if (asteroid->y == HEIGHT - 1) {
        asteroid->dy = -1;  // Só pode mover para cima
    }
    // Se o asteroide nasceu na esquerda, ele só pode mover para a direita
    if (asteroid->x == 0) {
        asteroid->dx = 1;   // Só pode mover para a direita
    }
    // Se o asteroide nasceu na direita, ele só pode mover para a esquerda
    if (asteroid->x == WIDTH - 1) {
        asteroid->dx = -1;  // Só pode mover para a esquerda
    }
}

void initializeAsteroids(Asteroid asteroids[MAX_ASTEROIDS]) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        // Inicializar cada asteroide com um contador para controle de tempo de "aparecimento"
        asteroids[i].active = 0; // Inicialmente inativo
        asteroids[i].counter = rand() % 50 + 1; // Aleatório entre 1 e 10 ciclos antes de aparecer
        asteroids[i].health = 1;
    }
}

void updateAsteroids(int map[HEIGHT][WIDTH], Asteroid asteroids[MAX_ASTEROIDS]) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active) {
            asteroids[i].counter--; // Decrementa o contador de ciclos

            // Se o contador chega a zero, o asteroide pode aparecer
            if (asteroids[i].counter <= 0) {
                // Inicializa o asteroide com posição e movimento
                setAsteroidPosition(&asteroids[i]);
                setAsteroidDirection(&asteroids[i]);
                asteroids[i].active = 1; // Torna o asteroide ativo
            }
        } else {
            // Limpa a posição antiga
            map[asteroids[i].y][asteroids[i].x] = 0;
            
            // Atualiza a posição do asteroide
            asteroids[i].x += asteroids[i].dx;
            asteroids[i].y += asteroids[i].dy;

            // Verifica se o asteroide saiu do mapa
            if (asteroids[i].x < 0) asteroids[i].x = WIDTH - 1;
            if (asteroids[i].x >= WIDTH) asteroids[i].x = 0;
            if (asteroids[i].y < 0) asteroids[i].y = HEIGHT - 1;
            if (asteroids[i].y >= HEIGHT) asteroids[i].y = 0;

            // Reposiciona o asteroide se tocar a borda
            if (asteroids[i].x == 0 || asteroids[i].y == 0 || asteroids[i].x == WIDTH - 1 || asteroids[i].y == HEIGHT - 1) {
                setAsteroidPosition(&asteroids[i]);
                setAsteroidDirection(&asteroids[i]);
            }

            // Atualiza o mapa com a posição do asteroide
            map[asteroids[i].y][asteroids[i].x] = 3; // Representa o asteroide
        }
    }
}

void handleBulletCollision(Asteroid *asteroid) {
    asteroid->active = 0; // Inicialmente inativo
    asteroid->counter = rand() % 10 + 1; // Aleatório entre 1 e 10 ciclos antes de aparecer
    asteroid->health = 1;
    //map[asteroid->x][asteroid->y] = 0;

}