#include <SFML/Graphics.h>
#include <SFML/System.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

void velocidade(float *vx, float *vy) {
    if (*vy > 0) {
       *vy -= 0.00005;
    } else if (*vy < 0) {
       *vy += 0.00005;
    }

    if (*vx > 0) {
        *vx -= 0.00005;
    } else if (*vx < 0) {
        *vx += 0.00005;
    }
}

int main() {
    bool gameover = false;

    //gerar janela
    sfRenderWindow * janela;
    sfVideoMode mode = {1024, 768, 32};
    janela = sfRenderWindow_create(mode, "Janelhina", sfResize | sfClose, NULL);
    sfColor fundo = sfBlack;

    //gerar texto
    sfFont *fonte = sfFont_createFromFile("CSFML_projeto/imgs/openSans/OpenSans-Bold.ttf");
    sfText* texto = sfText_create();
    sfText_setString(texto, "Game Over");
    sfText_setFont(texto, fonte);
    sfText_setCharacterSize(texto, 24);
    sfText_setFillColor(texto, sfWhite);
    sfVector2f textPosition = {300, 250};
    sfText_setPosition(texto, textPosition);
    
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

    //variáveis de posição e velocidade
    float x = 0.0, y = 0.0;
    float inimigo_X = 300, inimigo_y = 400;
    float vx = 0.0, vy = 0.0;

    sfFloatRect rect_Circulo = sfSprite_getLocalBounds(sCirculo);
    sfFloatRect rect_Circulo2 = sfSprite_getLocalBounds(sCirculo2);

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

        //Checa se alguma tecla foi pressionada e aumenta a velocidade
        if (sfKeyboard_isKeyPressed(sfKeyLeft))
            vx-=0.0001;
        if (sfKeyboard_isKeyPressed(sfKeyRight))
            vx+=0.0001;
        if (sfKeyboard_isKeyPressed(sfKeyDown))
            vy+=0.0001;
        if (sfKeyboard_isKeyPressed(sfKeyUp))
            vy-=0.0001;
        
        //Define a nova posição
        x += vx;
        y += vy;

        //balanceia a velocidade
        velocidade(&vx, &vy);
        

        sfVector2u sizeJanela = sfRenderWindow_getSize(janela);

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


        float dx = x + (rect_Circulo.width*0.1/2) - (inimigo_X + (rect_Circulo2.width*0.1/2));
        float dy = y + (rect_Circulo.height*0.1/2) - (inimigo_y + (rect_Circulo2.height*0.1/2));

        float Distancia = sqrt(pow(dx, 2) + pow(dy, 2));

        if (Distancia < (rect_Circulo.width*0.1/2) + (rect_Circulo2.width*0.1/2)) {
            gameover = true;
        }

        sfVector2f direcao = {x - inimigo_X, y - inimigo_y};

        
        
        inimigo_X += direcao.x*(100/Distancia*0.0005);
        inimigo_y += direcao.y*(100/Distancia*0.0005);
        


        

        sfRenderWindow_clear(janela, fundo);
        sfSprite_setPosition(sCirculo, (sfVector2f){x ,y});
        sfSprite_setPosition(sCirculo2, (sfVector2f){inimigo_X ,inimigo_y});

        if (gameover) {
            sfRenderWindow_drawText(janela, texto, NULL);
        }

        sfRenderWindow_drawSprite(janela, sCirculo, NULL);
        sfRenderWindow_drawSprite(janela, sCirculo2, NULL);
        sfRenderWindow_display(janela);
        if (gameover) {
            sfSleep(delayTime);
            sfRenderWindow_close(janela);
        }


    }


    sfText_destroy(texto);
    sfFont_destroy(fonte);
    sfTexture_destroy(tCirculo);
    sfSprite_destroy(sCirculo);
    sfSprite_destroy(sCirculo2);
    sfRenderWindow_destroy(janela);
    

    return 0;
}
