#include "../include/controller.h"
#include "../include/constants.h"
#include "../include/player.h"
#include "../include/bullet.h"
#include "../include/asteroid.h"
#include "../include/map.h"
#include <stdio.h>
#include <unistd.h>

int gameState = 0; // 0 = MENU, 1 = PLAYING, 2 = PAUSED, 3 = encerrar jogo, 4 = REINICIAR JOGOS
int running = 1;   // controla se o jogo está rodando 

pthread_mutex_t inputMutex;
pthread_mutex_t collisionMutex;

// Função para a Thread de captura de inputs
void *inputThread(void *arg) {
    struct {
        Player *player1;
        Player *player2;
        int (*map)[WIDTH];
        Bullet *bullets;
        __int16_t *X;
        int *value_X_Mouse;
        int *click_button;
        int *tiro_botao;
    } *context = arg;

    __int16_t *X = context->X;
    Player *player2 = context->player2;      // Ponteiro para o jogador
    Player *player1 = context->player1;      // Ponteiro para o jogador
    int (*map)[WIDTH] = context->map;     // Ponteiro para o mapa
    Bullet *bullets = context->bullets;    // Correção aqui para ponteiro
    int *value_mouse = context->value_X_Mouse; 
    int *click = context->click_button;
    int *tiro = context->tiro_botao;
    
    const int DEAD_ZONE = 10; // Define uma zona morta para evitar movimentos desnecessários
    
    
    while (running) {

        int currentDirection = 0;
        if (X[0] < -20) {
            currentDirection = -1; // Esquerda
        } else if (X[0] > 20) {
            currentDirection = 1; // Direita
        }
        int currentDirection2 = 0;
        if (*value_mouse < -10){
            currentDirection2 = -1;
        }else if (*value_mouse > 10){
            currentDirection2 =1;
        }
        // verifica se foi dado algum tiro pelo mouse
        if (*click == 1) {
            shootBullet(bullets, MAX_BULLETS, player2);  // Corrigido com ponto e vírgula
        }
        //verifica se foi dado um tiro pela placa
        if (*tiro == 1){
            shootBullet(bullets, MAX_BULLETS, player1);
            *tiro = 0;
        }

        // verifica para qual direção a nave vai ser movida
        if(currentDirection2 == -1){
            player2->dx = -1;
            movePlayer(player2,map);
        }
        else if (currentDirection2 == 1) {
            player2->dx = 1;
            movePlayer(player2, map);
        }
        
        if (currentDirection == -1) {
            player1->dx = -1;
            player1->dy = 0;
            movePlayer(player1, map);
        } else if (currentDirection == 1) {
            player1->dx = 1;
            player1->dy = 0;
            movePlayer(player1, map);
        }
    
    }
    usleep(100000);
    
    return NULL;
}


void* checkCollisions(void* arg) {
    CollisionContext *context = (CollisionContext*) arg;

    while (running) {
        // Bloqueia o acesso aos dados compartilhados enquanto a verificação de colisões acontece

        // Verifica as colisões entre as balas e os asteroides
        for (int i = 0; i < context->maxBullets; i++) {
            if (context->bullets[i].active) {  // Se a bala está ativa
                for (int j = 0; j < context->maxAsteroids; j++) {
                    if (checkBulletCollision(&context->bullets[i], &context->asteroids[j])) {
                        context->bullets[i].active = 0;
                        context->asteroids[j].active = 0;
                        context->map[context->bullets[i].dx][context->bullets[i].dy] = 0;
                        context->map[context->asteroids[j].dx][context->asteroids[j].dy] = 0;
                        handleBulletCollision(&context->asteroids[j]);
                        
                    }
                }
            }
        }
        usleep(100000); // Aguarda um pouco antes de verificar novamente
    }
    return NULL;
}

int checkCollision(Player *player, Asteroid *asteroid) {
    // Simples verificação de colisão (ajuste conforme a lógica do seu jogo)
    if (player->x == asteroid->x && player->y == asteroid->y && asteroid->active == 1) {
        return 1;  // Colisão ocorreu
    }
    return 0;  // Nenhuma colisão
}

int checkBulletCollision(Bullet *bullet, Asteroid *asteroid) {
    // Verifica se a bala e o asteroide estão na mesma posição
    if (bullet->x == asteroid->x && bullet->y == asteroid->y + 1 && bullet->active) {
        return 1; // Colisão ocorreu
    }
    return 0; // Sem colisão
}