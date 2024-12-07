#include "../src/map.c"
#include "../src/player.c"
#include "../src/asteroid.c"
#include "../src/bullet.c"
#include <unistd.h> 
#include <stdlib.h>
#include <pthread.h>
#include "../include/controller.h"
#include "../src/controller.c"
#include "../src/i2C.c"
#include <linux/input.h>
#include "create_sprite.c"
#include "telas_jogo.c"

I2C_Registers regs;
int fd;

__int16_t X[1];
int value_X_Mouse;
int click;
int tiro_botao;
void accelInit();
void* accelWorking(void *args);
void* mouseWorking(void *args);
void* button_threads(void *args);
void  zera_sprites();
void verifica_vida(Player *p);

int state = 0;
int pausegame = 0;
int iniciarjogo = 0;
int currentState;
int vida1 = 3;
int vida2 = 3;
int gameover=0;

extern void mem();
extern void save_sprite(const unsigned short pixel_vector[400], int offset);
extern void sprite( int offset,int Y, int X, int sp, int reg);
extern int push_button();
extern void background_color(int color);

int main() {
    mem();
    save_sprite(nave,25);
    background_color(0);
    save_sprite(meteoro, 26);
    srand(time(NULL));
    zera_sprites();
    accelInit();

    pthread_t threadAccel,threadMouse, threadButton, inputThreadId, collisionThreadId;
    pthread_mutex_init(&inputMutex, NULL);
    pthread_mutex_init(&collisionMutex, NULL);
    int map[HEIGHT][WIDTH];
    createMap(map);

    Player player1;
    player1.x = WIDTH / 2;      // colocar no centro horizontal
    player1.y = HEIGHT - 2; 
    Player player2;
    player2.x = WIDTH / 4;      // colocar no centro horizontal
    player2.y = HEIGHT - 2;

    inicializePlayer(&player1,1);
    inicializePlayer(&player2,2);

    player2.x = player2.x + 2;
    Asteroid asteroids[MAX_ASTEROIDS];
    initializeAsteroids(asteroids);

    Bullet bullets[MAX_BULLETS];
    initializeBullets(bullets, MAX_BULLETS);
    
    struct {
        Player *player2;
        Player *player1;
        int (*map)[WIDTH];
        Bullet *bullets;
        __int16_t *X;
        int *value_X_Mouse;
        int *click;
        int *tiro_botao;
    } inputContext = {&player2, &player1, map, bullets, X, &value_X_Mouse, &click, &tiro_botao};

    pthread_create(&inputThreadId, NULL, inputThread, &inputContext);
    pthread_create(&threadAccel, NULL, accelWorking, NULL);
    pthread_create(&threadMouse, NULL, mouseWorking, NULL);
    pthread_create(&threadButton, NULL, button_threads, NULL);


    // Definir o contexto para a thread de colisão
    CollisionContext colicontext = { bullets, asteroids, MAX_BULLETS, MAX_ASTEROIDS, map };

    // Criar a thread de colisão
    pthread_create(&collisionThreadId, NULL, checkCollisions, &colicontext);
    
    while (running) {

        currentState = gameState;

        if (currentState == 0) { // Tela inicial
            desenha_tela(tela_inicial); 
            usleep(100000); // Pausa para exibir o menu
        } else if (currentState == 1) { // Jogando
        //verifica_vida(&player1);
        updatePlayerOnMap(map, &player1);
        updatePlayerOnMap(map, &player2);
        updateAsteroids(map, asteroids);

        // Verifica colisão entre o jogador e os asteroides
        for (int i = 0; i < MAX_ASTEROIDS; i++) {
            if (checkCollision(&player1, &asteroids[i])) { // Checa colisão com asteroides
                int collisionStatus = handleCollisionWithAsteroid(&player1);
                if (collisionStatus == 0) {
                    // O jogador morreu
                    gameState = 5; // Finaliza o jogo
                    printf("Game Over! O jogador morreu.\n");
                    break;
                }
            }
        }
        for (int i = 0; i < MAX_ASTEROIDS; i++) {
            if (checkCollision(&player2, &asteroids[i])) { // Checa colisão com asteroides
                int collisionStatus = handleCollisionWithAsteroid(&player2);
                if (collisionStatus == 0) {
                    // O jogador morreu
                    gameState = 5; // Finaliza o jogo
                    printf("Game Over! O jogador morreu.\n");
                    break;
                }
            }
        }
        
        // Atualiza e desenha as balas
        
        printMap(map, asteroids, bullets, &player1, &player2);
        updateBullets(bullets, MAX_BULLETS, map);
        drawBullets(bullets, MAX_BULLETS, map);
        
        switch (player1.health)
        {
        case 3:
            sprite(15,220,1,1,27);
            sprite(15,240,1,1,28);
            sprite(15,260,1,1,29);
            break;
        case 2:
            sprite(15,220,1,0,27);
            break;
        case 1:
            sprite(15,240,1,0,28);
            break;
        default:
            break;
        }

        switch (player2.health)
        {
        case 3:
            sprite(15,220,620,1,24);
            sprite(15,240,620,1,25);
            sprite(15,260,620,1,26);
            break;
        case 2:
            sprite(15,220,620,0,24);
            sprite(15,240,620,1,25);
            sprite(15,260,620,1,26);
            break;
        case 1:
            sprite(15,220,620,0,24);
            sprite(15,240,620,0,25);
            sprite(15,260,620,1,26);
            break;
        default:
            break;
        }
        
        usleep(100000); // Controla a velocidade do jogo
        } 
        
        else if (currentState == 4){ // reinicia jogo
            restartGame(&player1,&player2,map,asteroids,bullets);
            vida1 = 3;
            vida2 = 3;
            gameState =0;
            gameover = 0;
        }
        else if (currentState == 5){
            desenha_tela(telafim);
            zera_sprites();
        }
    }

    // Finaliza as threads e libera recursos
    pthread_join(inputThreadId, NULL);
    pthread_join(collisionThreadId, NULL);
    pthread_join(threadMouse, NULL);
    pthread_join(threadAccel, NULL);
    pthread_join(threadButton, NULL);
    pthread_mutex_destroy(&inputMutex);
    pthread_mutex_destroy(&collisionMutex);

    printf("Jogo encerrado.\n");
    return 0;
}

void* accelWorking(void *args) {
    while (running) {
        if (accelereometer_isDataReady(regs)) {
            accelerometer_x_read(X, regs);
        }
        usleep(100000);
    }
    return NULL;
}

void accelInit() {
        fd = open_fd();
        regs = map_i2c(fd);
        I2C0_Init(&regs);
        accelerometer_init(regs);
}

void* mouseWorking(void *args){
    fd = open("/dev/input/event0",O_RDONLY);
    if (fd == -1){
        exit(1);
    }
    struct input_event ev;
    while (running) {

        ssize_t bytesRead = read(fd, &ev, sizeof(struct input_event));
        if (bytesRead < 0) {
            perror("Erro ao ler evento");
            close(fd);
        }       
        if (ev.type == EV_REL) {
            if (ev.code == REL_X) {
                value_X_Mouse +=  ev.value;
            }
        } 
        if (ev.type == EV_KEY) {
            if (ev.code == BTN_LEFT) {
                click = ev.value;
                
            }
        }
    }
    //usleep(10000);
    close(fd);
    return NULL;
}

void zera_sprites(){
    for (int i =0; i < 32; i++){
        sprite(15, 1, 1, 0, i);
        sprite(25, 1 , 1 , 0, i);
        sprite(5, 1 , 1 , 0, i);
        sprite(11, 1 , 1 , 0, i);
        sprite(26, 1 , 1 , 0, i);
    }
}

// função para verificar se o botão foi pressionado usando threads
void* button_threads(void *args) {
    while (running) {
        int teste;
        if (state == 0) {
            teste = push_button();
            switch (teste) {
                case 14:  // inicia o jogo caso o botão 1 seja pressionado
                    
                    gameState = 1;
                    state = 1;
                    clear_screen();
                    desenha_tela(tela_jogo);
        
                    break;
                case 11:   // encerra o programa se o botão 11 foi pressionado
                    
                    gameState = 3;
                    running = 0;
                    clear_screen();
                    desenha_tela(telafim);
        
                    exit(0);
                    break;
                default:
                    break;
            }
            usleep(100000);
        } else if (state == 1) {
            teste = push_button();
            switch (teste) {
                case 14: // o player 1 atira com o botão 14 pressionado
                    tiro_botao = 1;
                    break;
                case 13: // pausa o jogo caso o botão 2 seja pressionado
                    
                    if (gameState != 2) {
                        gameState = 2;
                        
                        clear_screen();
                        zera_sprites();
                        desenha_tela(tela_pause);
                    } else {
                        gameState = 1;
                        clear_screen();
                        desenha_tela(tela_jogo);
                    }
        
                    break;
                 case 11:   // reinicia o programa se o botão 3 foi pressionado
                    
                    gameState = 3;
                    running = 0;
                    clear_screen();
                    zera_sprites();
                    desenha_tela(telafim);
        
                    exit(0);
                    break;
                case 7: // reinicia o jogo caso o botão 4 seja pressionado
                    gameState = 4;
                    clear_screen();
                    zera_sprites();
                    state = 0;
                    break;
                default:
                    break;
            }
            usleep(100000);
        }
        
    }
    return NULL;
}


