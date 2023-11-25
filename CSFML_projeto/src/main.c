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
    int pontos = 0;

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
    
    //textura dos Circulos
    sfTexture * tCirculo;
    tCirculo = sfTexture_createFromFile("CSFML_projeto/imgs/circulo.png", NULL);

    //Sprite do Circulo do player
    sfSprite * sCirculo;
    sCirculo = sfSprite_create();
    sfSprite_setTexture(sCirculo, tCirculo, 0);
    sfSprite_setScale(sCirculo, (sfVector2f){0.1 ,0.1});

    //Sprite do circulo inimigo
    sfSprite * sCirculo2;
    sCirculo2 = sfSprite_create();
    sfSprite_setTexture(sCirculo2, tCirculo, 0);
    sfSprite_setScale(sCirculo2, (sfVector2f){0.1 ,0.1});
    sfSprite_setColor(sCirculo2, sfColor_fromRGBA(255, 255, 0, 255));

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
    float inimigo_X = 300, inimigo_y = 400;
    float moeda_X = 400, moeda_y = 400;
    
    //Hitbox do sprites
    sfFloatRect rect_Circulo = sfSprite_getLocalBounds(sCirculo);
    sfFloatRect rect_Circulo2 = sfSprite_getLocalBounds(sCirculo2);
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

        // diferença entre as posições
        float dx = x + (rect_Circulo.width*0.1/2) - (inimigo_X + (rect_Circulo2.width*0.1/2));
        float dy = y + (rect_Circulo.height*0.1/2) - (inimigo_y + (rect_Circulo2.height*0.1/2));

        // Hipotenusa
        float Distancia = sqrt(pow(dx, 2) + pow(dy, 2));
        //caso eles se toquem
        if (Distancia < (rect_Circulo.width*0.1/2) + (rect_Circulo2.width*0.1/2)) {
            gameover = true;
        }

        sfVector2f direcao = {x - inimigo_X, y - inimigo_y};
        
        inimigo_X += direcao.x*(100/Distancia*0.01);
        inimigo_y += direcao.y*(100/Distancia*0.01);

        // diferença entre as posições entre x e a moeda
        float dxM = x + (rect_Circulo.width*0.1/2) - (moeda_X + (rect_Moeda.width*0.3/2));
        float dyM = y + (rect_Circulo.height*0.1/2) - (moeda_y + (rect_Moeda.height*0.3/2));

        // Hipotenusa moeda
        float Distancia_moeda = sqrt(pow(dxM, 2) + pow(dyM, 2));
        //caso eles x e moeda se toquem
        if (Distancia_moeda < (rect_Circulo.width*0.1/2) + (rect_Moeda.width*0.3/2)) {
            pontos++;
            moeda_X = (rand() % 1024) - 37;
            moeda_y = (rand() % 768) - 37;
        }
        


        
        //posiciona os sprites
        sfRenderWindow_clear(janela, fundo);
        sfSprite_setPosition(sCirculo, (sfVector2f){x ,y});
        sfSprite_setPosition(sCirculo2, (sfVector2f){inimigo_X ,inimigo_y});
        sfSprite_setPosition(sMoeda, (sfVector2f){moeda_X ,moeda_y});


        //Renderiza a tela
        sfRenderWindow_drawSprite(janela, sCirculo, NULL);
        sfRenderWindow_drawSprite(janela, sCirculo2, NULL);
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

    sfText_destroy(textoPontos);
    sfFont_destroy(fonte);
    sfTexture_destroy(tMoeda);
    sfSprite_destroy(sMoeda);
    sfTexture_destroy(tCirculo);
    sfSprite_destroy(sCirculo);
    sfSprite_destroy(sCirculo2);
    sfRenderWindow_destroy(janela);
    

    return 0;
}
