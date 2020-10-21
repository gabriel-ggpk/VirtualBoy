#include <raylib.h>

#include "faseAbertura.c"
#include "Boss.c"
#include "faseNave.c"
#include "fase2.c"
#include "faseRunner.c"

#define PHYSAC_IMPLEMENTATION
#define PHYSAC_NO_THREADS
#include "physac.h"


int main(){
    int windowWidth = 1280, windowHeight = 720;
    int fase = 0;
    int gemasColetadas=0;

    InitWindow(windowWidth, windowHeight, "Não temos um nome");
    InitAudioDevice();

    //botao recursos
    Texture2D button = LoadTexture("assets/Button.png");
    button.width = button.width*5;
    button.height = button.height*5;
    Rectangle botaoFrame;
    botaoFrame.width = button.width/2;
    botaoFrame.height = button.height/3;
    botaoFrame.x = botaoFrame.width*0;
    botaoFrame.y = botaoFrame.height*1;
    Rectangle botaoArea = { GetScreenWidth()/2 -botaoFrame.width/2, 400, button.width/2, botaoFrame.height};
    Vector2 mousePoint = { 0.0f, 0.0f };
    int botaoStatus = 0;
    int frasePosi = botaoArea.y +25;

    SetTargetFPS(60);
    while(!WindowShouldClose()){
        switch(fase){
           case 0:
            if(IsKeyPressed(KEY_ONE))
                fase = 1;
            if(IsKeyPressed(KEY_TWO))
                fase = 2;
            if(IsKeyPressed(KEY_THREE))
                fase = 3;
            if(IsKeyPressed(KEY_FOUR))
                fase = 4;

            mousePoint = GetMousePosition();
            if (CheckCollisionPointRec(mousePoint, botaoArea)){
                if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                    botaoStatus = 1;
                if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) 
                    botaoStatus = 2;
            }
            else
                botaoStatus = 0;


            if(botaoStatus == 1){
                frasePosi = botaoArea.y +32;
                botaoFrame.x = botaoFrame.width; 
                
            }
            if(botaoStatus == 2){
                frasePosi = botaoArea.y +25;
                botaoFrame.x = 0;
                fase = -1;
                botaoStatus = 0;
            }
            
            BeginDrawing();
                ClearBackground(SKYBLUE);
                DrawText("VirtualGuy", 200, 50, 150, BLACK);
                DrawText("Escolha uma fase 1, 2, 3 ou 4", 400, windowHeight -20, 20, BLACK);
                
                DrawTextureRec(button, botaoFrame, (Vector2){ botaoArea.x, botaoArea.y }, WHITE);
                DrawText("Jogar", botaoArea.x +20, frasePosi, 40, WHITE);
            EndDrawing();
            break;

            case -1:
             if(abertura(gemasColetadas) == 0) fase = 1;
            else if(abertura(gemasColetadas) == 1)fase = 2;
            else if(abertura(gemasColetadas) == 2)fase = 3;
            else if(abertura(gemasColetadas) == 3)fase = 4;
            else if(abertura(gemasColetadas) == 4)  fase = 0;

             break;

           case 1:
            if(faseRunner() == 1){
                fase = -1;
                gemasColetadas=1;
            }
            else if(faseRunner() == 2) fase = 2;
            break;

           case 2:
            if(Fase2() == 2){
                fase = -1;
                gemasColetadas=2;
            }
            else if(Fase2()==3) fase = 0;
            else fase = 2;
            break;

           case 3:
            if(Fase3() == 1){
                fase = 0;
            }
            else if(Fase3() == 2){
                fase = -1;
                gemasColetadas=3;
            }
            break;

           case 4:
            if(BOSS() == 1){
                fase = -1;
                gemasColetadas=4;
            }
            break;
        }
    }

    UnloadTexture(button);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
