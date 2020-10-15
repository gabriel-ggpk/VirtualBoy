#include <raylib.h>

#include "Boss.c"
#include "naveFase.c"
#include "faseRunner.c"

int main(){
    int windowWidth = 1280, windowHeight = 720;
    int fase = 0;

    InitWindow(windowWidth, windowHeight, "Não temos um nome");

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
            BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("Escolha uma fase 1, 2, 3 ou 4", 150, windowHeight/2, 20, BLACK);
            EndDrawing();
            break;

           case 1:
            if(faseRunner() == 1){
                fase = 0;
            }
            break;

           case 2:
            if(IsKeyPressed(KEY_ZERO))
                fase = 0;
            BeginDrawing();
                DrawText("Aperte 0 para voltar", 150, windowHeight/2, 20, BLACK);
                ClearBackground(BLUE);
            EndDrawing();
            break;

           case 3:
            if(Fase3() == 1){
                fase = 0;
            }
            else if(Fase3() == 2){
                fase = 4;
            }
            break;

           case 4:
            if(BOSS() == 1){
                fase = 0;
            }
            break;
        }
    }

    CloseWindow();

    return 0;
}
