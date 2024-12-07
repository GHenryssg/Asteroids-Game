#include "../include/map.h"


extern void sprite( int offset,int Y, int X, int sp, int reg);



void apaga_bala(){
    sprite(11, 1 , 1 , 0, 7);
    sprite(11, 1 , 1 , 0, 8);
    sprite(11, 1 , 1 , 0, 9);
    sprite(11, 1 , 1 , 0, 10);
    sprite(11, 1 , 1 , 0, 11);
}

void apaga_asteroids(){
    sprite(20, 1 , 1 , 0, 2);
    sprite(20, 1 , 1 , 0, 3);
    sprite(20, 1 , 1 , 0, 4);
}

void createMap(int map[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            map[i][j] = 0;
        }
    }
}

void printMap(int map[HEIGHT][WIDTH], Asteroid ast[MAX_ASTEROIDS], Bullet b[MAX_BULLETS], Player *p1, Player *p2) {
    tiro = 0;
    asteroidss =0;
    jogador = 0;
   
    /*
    Neste for ele varre a matriz e verifica quais os elementos presentes para exibir os sprites
    */
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            
            if (map[i][j] == 2){
                sprite(5, i * 20 , j * 20 , 1,30);                
            }
            else if(map[i][j] == 1){
                sprite(25, i * 20 , j * 20 , 1, 31);
            }

            if(map[i][j] == 3 && ast->active == 1){
                switch (asteroidss)
                {
                case 0:
                    sprite(26, i*20 , j * 20 , 1, 2);
                    asteroidss++;
                    break;
                case 1:
                    sprite(26, i*20 , j * 20 , 1, 3);
                    asteroidss++;
                    break;
                case 2: 
                    sprite(26, i*20 , j * 20 , 1, 4);
                    asteroidss++;
                    break;
                default:
                    break;
                }
            }
            else if(map[i][j] == 3 && ast->active==0){
                map[i][j] =0;
            }
            if(map[i][j] == 5 && b->active == 1){
                switch (tiro)
                {
                case 0:
                    sprite(11, i*20 , j * 20 , 1, 7);
                    tiro++;
                    break;
                case 1:
                    sprite(11, i*20 , j * 20 , 1, 8);
                    tiro++;
                    break;
                case 2: 
                    sprite(11, i*20 , j * 20 , 1, 9);
                    tiro++;
                    break;
                case 3:
                    sprite(11, i*20 , j * 20 , 1, 10);
                    tiro++;
                    break;
                case 4:
                    sprite(11, i*20 , j * 20 , 1, 11);
                    tiro++;
                    break;
                default:
                    break;
                }
            }else if (map[i][j] == 5 && b->active == 0){
                map[i][j] = 0;
            }
    }
    }
}
