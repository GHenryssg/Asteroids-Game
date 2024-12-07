#include "../include/player.h"
#include "../include/map.h"
#include "../include/bullet.h"
#include "../include/asteroid.h"

// Função para inicializar o jogador
void inicializePlayer(Player *player, int num) {
    //player->x = WIDTH / 2;      // colocar no centro horizontal
    //player->y = HEIGHT - 2;     // coloca perto do fundo 
    player->dx = 0;
    player->dy = 0;
    player->speed = 1;
    player->active = 1;
    player->health = 3;
    player->id = num;
    //player->limite_bullets = MAX_BULLETS/2;
}

void restartGame(Player *player1, Player *player2, int map[HEIGHT][WIDTH], Asteroid *asteroids, Bullet *bullets) {
    // Reinicializa o mapa
    createMap(map); 

    // Reinicializa o jogador
    inicializePlayer(player1, player1->id);
    inicializePlayer(player2, player2->id);

    // Se houver outros elementos como balas ou asteroides, reinicie-os aqui
    // Exemplo: reinicializar asteroides ou balas
    initializeAsteroids(asteroids);
    initializeBullets(bullets, MAX_BULLETS);
    
    printf("Jogo reiniciado!\n");
}

void movePlayer(Player *player, int map[HEIGHT][WIDTH]) {
    // Salva a posição anterior do jogador
    int prevX = player->x;
    int prevY = player->y;

    // Atualiza a posição com base na direção e velocidade
    player->x += player->dx;
    player->y += player->dy;

    // Limita o jogador para não sair do mapa
    if (player->x < 0) player->x = 0;
    if (player->x >= WIDTH) player->x = WIDTH - 1;
    if (player->y < 0) player->y = 0;
    if (player->y >= HEIGHT) player->y = HEIGHT - 1;

    // Apaga a posição anterior do jogador no mapa
    if (prevX >= 0 && prevX < WIDTH && prevY >= 0 && prevY < HEIGHT) {
        map[prevY][prevX] = 0;  // Marca como vazio
    }
    usleep(100000);
}


void updatePlayerOnMap(int map[HEIGHT][WIDTH], Player* player) {
    // Atualiza a posição do jogador no mapa
    if (player->x >= 0 && player->x < WIDTH && player->y >= 0 && player->y < HEIGHT) {
        map[player->y][player->x] = player->id;  // Marca a nova posição do jogador
    }
}




// Retorna 1 se o jogador foi atingido, 0 se o jogador continua jogando
int handleCollisionWithAsteroid(Player *player) {
    if (player->active) {
        player->health -= 1; // Diminui a vida do jogador
        if(player->id == 1){
            vida1 --;
        }
        else if(player->id == 2){
            vida2 --;
        }
        
        if (player->health <= 0) {
            player->active = 0; // Desativa o jogador
            return 0; // O jogador morreu
        } else {
            printf("O jogador foi atingido! Vida restante: %d\n", player->health);
            return 1; // O jogador ainda está vivo
        }
    }
    return 1; // Caso o jogador ainda esteja ativo
}
