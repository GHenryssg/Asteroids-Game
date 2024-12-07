#include "../include/bullet.h"
#include "../include/constants.h"
#include "../include/player.h"

void initializeBullets(Bullet bullets[], int maxBullets) {
    for (int i = 0; i < maxBullets; i++) {
        bullets[i].x = -1;       // Inicializa fora da tela
        bullets[i].y = -1;
        bullets[i].active = 0;   // Todas as bullets começam inativas
    }
}

void shootBullet(Bullet bullets[], int maxBullets, Player *player) {
    for (int i = 0; i < maxBullets; i++) {
        if (!bullets[i].active) { // Procura por uma bullet inativa
            // Posição inicial da bala deslocada à frente do jogador
            bullets[i].x = player->x;
            bullets[i].y = player->y -1;

            // Define a direção da bala com base na direção do jogador
            bullets[i].dx = 0;
            bullets[i].dy = -1;

            // Se o jogador estiver parado, a bala dispara para cima por padrão
            if (bullets[i].dx == 0 && bullets[i].dy == 0) {
                bullets[i].dy = -1; // Dispara para cima
                bullets[i].x = player->x;
                bullets[i].y = player->y - 1; // Uma célula acima
            }

            // Verifica se a posição inicial da bala está dentro do mapa
            if (bullets[i].x >= 0 && bullets[i].x < WIDTH &&
                bullets[i].y >= 0 && bullets[i].y < HEIGHT) {
                bullets[i].active = 1; // Ativa a bala
            }

            break;
        }
    }
}

void updateBullets(Bullet bullets[], int maxBullets, int map[HEIGHT][WIDTH]) {
    for (int i = 0; i < maxBullets; i++) {
        if (bullets[i].active) {
            // Remove a bala da posição antiga na matriz
            if (bullets[i].y >= 0 && bullets[i].y < HEIGHT && 
                bullets[i].x >= 0 && bullets[i].x < WIDTH) {
                map[bullets[i].y][bullets[i].x] = 0;
            }

            // Move a bala na direção definida
            bullets[i].x += bullets[i].dx;
            bullets[i].y += bullets[i].dy;

            // Verifica se a bala saiu dos limites
            if (bullets[i].x < 0 || bullets[i].x >= WIDTH || 
                bullets[i].y < 0 || bullets[i].y >= HEIGHT) {
                bullets[i].active = 0; // Desativa a bala
            } else {
                // Atualiza a posição da bala na matriz
                map[bullets[i].y][bullets[i].x] = 3; // 3 representa uma bala
            }
        }
    }
}

void drawBullets(Bullet bullets[], int maxBullets, int map[HEIGHT][WIDTH]) {
    for (int i = 0; i < maxBullets; i++) {
        if (bullets[i].active) {
            if (bullets[i].y >= 0 && bullets[i].y < HEIGHT && 
                bullets[i].x >= 0 && bullets[i].x < WIDTH ) {
                map[bullets[i].y][bullets[i].x] = 5; // 3 representa balas
            }
        }
    }
}
