#include <SFML/Graphics.h>
#include <SFML/System.h>
#include <SFML/Audio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void velocidade(float *vx, float *vy) {
    if (*vy > 0) {
       *vy -= 0.03;
    } else if (*vy < 0) {
       *vy += 0.03;
    }

    if (*vx > 0) {
        *vx -= 0.03;
    } else if (*vx < 0) {
        *vx += 0.03;
    }
}

int main() {
    srand(time(NULL));
    bool gameover = false;
    int pontos = 0, aux_pontos = 0;
    int inimigos = 1;

    //gerar janela
    sfRenderWindow * janela;
    sfVideoMode mode = {1024, 768, 32};
    janela = sfRenderWindow_create(mode, "Janelhina", sfResize | sfClose, NULL);
    sfColor fundo = sfBlack;

    //Limita o fps
    sfClock* clock = sfClock_create();
    sfTime timePerFrame = sfMilliseconds(1000/60); // Definir o tempo por frame desejado (60 FPS)
    sfTime elapsedTime = sfTime_Zero;

    //gerar texto
    sfFont *fonte = sfFont_createFromFile("CSFML_projeto/imgs/openSans/OpenSans-Bold.ttf");
    sfText* texto = sfText_create();
    sfText_setString(texto, "Game Over");
    sfText_setFont(texto, fonte);
    sfText_setCharacterSize(texto, 24);
    sfText_setFillColor(texto, sfWhite);
    sfVector2f textPosition = {1024/2-70, 768/2};
    sfText_setPosition(texto, textPosition);

    sfText * textoPontos = sfText_create();
    sfText_setFont(textoPontos, fonte);
    sfText_setCharacterSize(textoPontos, 30);
    sfText_setFillColor(textoPontos, sfWhite);

    //textura do background
    sfTexture * tBackground;
    tBackground = sfTexture_createFromFile("CSFML_projeto/imgs/fundo.png",NULL);    
    sfSprite * sBackground = sfSprite_create();
    sfSprite_setTexture(sBackground, tBackground, sfTrue);
    sfSprite_setPosition(sBackground, (sfVector2f){0,0});

    // Escala o sprite para preencher a janela
    sfVector2u tamanho_Bg = sfTexture_getSize(tBackground);
    
    //textura dos Circulos
    sfTexture * tCirculo;
    tCirculo = sfTexture_createFromFile("CSFML_projeto/imgs/circulo.png", NULL);

    //Sprite do Circulo do player
    sfSprite * sCirculo;
    sCirculo = sfSprite_create();
    sfSprite_setTexture(sCirculo, tCirculo, 0);
    sfSprite_setScale(sCirculo, (sfVector2f){0.1 ,0.1});

    //Sprite do circulo inimigo
    sfSprite * sCirculo2[10];
    for (int i = 0; i < 10; i++)
    {
        sCirculo2[i] = sfSprite_create();
        sfSprite_setTexture(sCirculo2[i], tCirculo, 0);
        sfSprite_setScale(sCirculo2[i], (sfVector2f){0.1 ,0.1});
        sfSprite_setColor(sCirculo2[i], sfColor_fromRGBA(255, 0, 0, 255));
    }

    //textura da moeda
    sfTexture * tMoeda;
    tMoeda = sfTexture_createFromFile("CSFML_projeto/imgs/moeda.png", NULL);

    //Sprite da moeda
    sfSprite * sMoeda;
    sMoeda = sfSprite_create();
    sfSprite_setTexture(sMoeda, tMoeda, 0);
    sfSprite_setScale(sMoeda, (sfVector2f){0.3 ,0.3});

    //variáveis de posição e velocidade
    float x = 0.0, y = 0.0;
    float vx = 0.0, vy = 0.0;
    float inimigo_X[10], inimigo_y[10];
    inimigo_X[0] = 700;
    inimigo_y[0] = 700;
    float moeda_X = 400, moeda_y = 400;
    
    //Hitbox do sprites
    sfFloatRect rect_Circulo = sfSprite_getLocalBounds(sCirculo);
    sfFloatRect rect_Circulo2[10];
    for (int i = 0; i < 10; i++) {
        rect_Circulo2[i] = sfSprite_getLocalBounds(sCirculo2[i]);
    }
    
    sfFloatRect rect_Moeda = sfSprite_getLocalBounds(sMoeda);

    // Define o tempo de atraso para 2 segundos
    sfTime delayTime = sfMilliseconds(2000);

    while (sfRenderWindow_isOpen(janela)) {
        //Evento de fechar a janela
        sfEvent evento;
        while(sfRenderWindow_pollEvent(janela, &evento)){
            if (evento.type == sfEvtClosed) {
              sfRenderWindow_close(janela);
            }
     
        }

        elapsedTime = sfClock_restart(clock);

        // Garantir que o tempo de espera entre frames seja cumprido
        while (sfTime_asMilliseconds(elapsedTime) < sfTime_asMilliseconds(timePerFrame)) {
            elapsedTime = sfClock_getElapsedTime(clock);
        }

        //Mostra os pontos na tela
        char pontosStr[50];
        sprintf(pontosStr, "Pontos: %d", pontos);
        sfText_setString(textoPontos, pontosStr);
        sfText_setPosition(textoPontos, (sfVector2f){10, 10});

        //Checa se alguma tecla foi pressionada e aumenta a velocidade
        if (sfKeyboard_isKeyPressed(sfKeyLeft))
            vx-=0.05;
        if (sfKeyboard_isKeyPressed(sfKeyRight))
            vx+=0.05;
        if (sfKeyboard_isKeyPressed(sfKeyDown))
            vy+=0.05;
        if (sfKeyboard_isKeyPressed(sfKeyUp))
            vy-=0.05;
        
        //Define a nova posição
        x += vx;
        y += vy;

        //balanceia a velocidade
        velocidade(&vx, &vy);
        

        sfVector2u sizeJanela = sfRenderWindow_getSize(janela);


        //Colisão com as bordas da janela
        if(y + rect_Circulo.height*0.1 > sizeJanela.y){
            y = sizeJanela.y - rect_Circulo.height*0.1;
            vy = - vy*0.1;
        } else if (y < 0) {
            y = 0;
            vy = - vy*0.1;
        }if (x < 0) {
            x = 0;
            vx = - vx*0.1;
        } else if (x + rect_Circulo.width*0.1 > sizeJanela.x) {
            x = sizeJanela.x - rect_Circulo.width*0.1;
            vx = - vx*0.1;
        }

        float dx[10], dy[10], Distancia[10];

        for (int i = 0; i <= inimigos-1; i++)  {
            dx[i] = x + (rect_Circulo.width*0.1/2) - (inimigo_X[i] + (rect_Circulo2[i].width*0.1/2));
            dy[i] = y + (rect_Circulo.height*0.1/2) - (inimigo_y[i] + (rect_Circulo2[i].height*0.1/2));
            Distancia[i] = sqrt(pow(dx[i], 2) + pow(dy[i], 2));
        }

        for (int i = 0; i <= inimigos-1; i++)  {
            if (Distancia[i] < (rect_Circulo.width*0.1/2) + (rect_Circulo2[i].width*0.1/2)) {
                gameover = true;
            }

            sfVector2f direcao = {x - inimigo_X[i], y - inimigo_y[i]};

            for (int j = 0; j <= inimigos-1; j++) {
                if(i != j){
                    // diferença entre as posições
                    dx[i] = inimigo_X[i] + (rect_Circulo2[i].width*0.1/2) - (inimigo_X[j] + (rect_Circulo2[j].width*0.1/2));
                    dy[i] = inimigo_y[i] + (rect_Circulo2[i].height*0.1/2) - (inimigo_y[j] + (rect_Circulo2[j].height*0.1/2));

                    // Hipotenusa
                    float distancia_inimigos = sqrt(pow(dx[i], 2) + pow(dy[i], 2));
                    //caso eles se toquem
                    if (distancia_inimigos < (rect_Circulo2[i].width*0.1/2) + (rect_Circulo2[j].width*0.1/2)+40) {
                        if (Distancia[i]>Distancia[j])
                        {
                            direcao.x =  direcao.x*vx*0.1;
                            direcao.y =  direcao.y*vy*0.1;
                        }
                        
                    }
                }
            }

            inimigo_X[i] += direcao.x*(100/Distancia[i]*0.01);
            inimigo_y[i] += direcao.y*(100/Distancia[i]*0.01);
            sfSprite_setPosition(sCirculo2[i], (sfVector2f){inimigo_X[i] ,inimigo_y[i]});
        }
        


        // diferença entre as posições entre x e a moeda
        float dxM = x + (rect_Circulo.width*0.1/2) - (moeda_X + (rect_Moeda.width*0.3/2));
        float dyM = y + (rect_Circulo.height*0.1/2) - (moeda_y + (rect_Moeda.height*0.3/2));

        // Hipotenusa moeda
        float Distancia_moeda = sqrt(pow(dxM, 2) + pow(dyM, 2));
        //caso eles x e moeda se toquem
        if (Distancia_moeda < (rect_Circulo.width*0.1/2) + (rect_Moeda.width*0.3/2)) {
            pontos++;
            aux_pontos++;
            moeda_X = (rand() % 987);
            moeda_y = (rand() % 731);
        }

        if (pontos == inimigos*3 && pontos == aux_pontos-inimigos+1)
        {
         inimigos++;
         aux_pontos++;
         float spawnX, spawnY;
         do {
            spawnX = (rand() % 1324 - 100);
            spawnY = (rand() % 731 - 100);
         } while (spawnX*spawnY >= 0 && spawnX*spawnY <= 1024*768);
         

         inimigo_X[inimigos-1] = spawnX;  
         inimigo_y[inimigos-1] = spawnY;  
        }
        
        


        
        //posiciona os sprites
        sfRenderWindow_clear(janela, fundo);
        sfSprite_setPosition(sCirculo, (sfVector2f){x ,y});
        sfSprite_setPosition(sMoeda, (sfVector2f){moeda_X ,moeda_y});


        //Renderiza a tela
        for(int i = 0; i <sfWindow_getSize(janela).y; i += tamanho_Bg.y){
            for(int j = 0; j < sfWindow_getSize(janela).x; j += tamanho_Bg.x){
            // Define a posição do sprite para desenhar a textura  
            sfSprite_setPosition(sBackground, (sfVector2f){(float)j, (float)i});
            sfRenderWindow_drawSprite(janela, sBackground, NULL);          
            }
        }
        sfRenderWindow_drawSprite(janela, sBackground, NULL);
        sfRenderWindow_drawSprite(janela, sCirculo, NULL);
        for (int i = 1; i <= inimigos; i++)  {
            sfRenderWindow_drawSprite(janela, sCirculo2[i-1], NULL);
        }
        sfRenderWindow_drawSprite(janela, sMoeda, NULL);
        sfRenderWindow_drawSprite(janela, textoPontos, NULL);
        if (gameover) {
            sfRenderWindow_drawText(janela, texto, NULL);
        }
        sfRenderWindow_display(janela);

        if (gameover) {
            sfSleep(delayTime);
            sfRenderWindow_close(janela);
        }


    }

    sfClock_destroy(clock);
    sfText_destroy(texto);
    sfTexture_destroy(tBackground);
    sfSprite_destroy(sBackground);
    sfText_destroy(textoPontos);
    sfFont_destroy(fonte);
    sfTexture_destroy(tMoeda);
    sfSprite_destroy(sMoeda);
    sfTexture_destroy(tCirculo);
    sfSprite_destroy(sCirculo);
        for (int i = 0; i < 10; i++)
    {
        sfSprite_destroy(sCirculo2[i]);
    }
    sfRenderWindow_destroy(janela);
    

    return 0;
}
